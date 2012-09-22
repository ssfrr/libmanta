#include "extern/hidapi/hidapi/hidapi.h"
#include <cassert>
#include "MantaUSB.h"
#include "MantaExceptions.h"
#include <cstdlib>
#include <cstring>
#include <wchar.h>

MantaUSB::MantaUSB(void) :
   SerialNumber(0),
   DeviceHandle(NULL)
{
   mantaList.push_back(this);
   MantaIndex = mantaList.size();

   DebugPrint("%s-%d: Manta %d initialized", __FILE__, __LINE__, MantaIndex);
}

MantaUSB::~MantaUSB(void)
{
   Disconnect();
   mantaList.remove(this);
   if(mantaList.empty())
   {
      hid_exit();
   }
}

bool MantaUSB::MessageQueued(void)
{
   return GetQueuedTxMessage() != NULL;
}

/************************************************************************//**
 * \brief Writes a USB transfer frame down to the Manta
 * \param   frame          Pointer to the frame to be transmitted
 * \param   forceQueued    Forces this message to be queued instead of merged
 *
 * WriteFrame() is meant to be called by the Manta subclass, which defines
 * methods for the individual messages (setLED, etc). libmanta maintains a
 * message queue that gets popped from in the HandleEvents() handler.
 *
 * The default behavior is that if a message is already queued up for a given
 * Manta, subsequent message will be merged into the waiting message instead of
 * being further queued (the queued frame will be the end result of all queued
 * messages). forceQueued can be set to true to force the message to be queued
 * as a separate message instead of being merged
 *
 * Note: Because WriteFrame() accesses the same message queue that
 * HandleEvents() does, they should be protected from each other by a mutex on
 * the application level if they're being called from parallel threads.
 ****************************************************************************/
void MantaUSB::WriteFrame(uint8_t *frame, bool forceQueued)
{
   if(NULL == DeviceHandle)
   {
      throw(MantaNotConnectedException(this));
   }
   MantaTxQueueEntry *queuedMessage = GetQueuedTxMessage();
   if(queuedMessage && !forceQueued)
   {
      /* replace the queued packet payload with the new one */
      for(int i = 0; i < OutPacketLen; ++i)
      {
         /* the first byte of the report is the report ID (0x00) */
         queuedMessage->OutFrame[i+1] = frame[i];
      }
      DebugPrint("%s-%d: (WriteFrame) Queued Transfer overwritten on Manta %d",
            __FILE__, __LINE__, GetSerialNumber());
   }
   else
   {
      /* no transfer in progress, queue up a new one */
      MantaTxQueueEntry *newMessage = new MantaTxQueueEntry;
      newMessage->OutFrame[0] = 0;
      newMessage->TargetManta = this;
      /* the first byte of the report is the report ID (0x00) */
      memcpy(newMessage->OutFrame + 1, frame, OutPacketLen);
      txQueue.push_back(newMessage);
      DebugPrint("%s-%d: (WriteFrame) Transfer Queued on Manta %d",
            __FILE__, __LINE__, GetSerialNumber());
   }
}

/************************************************************************//**
 * \brief Queries connection status of the Manta
 * \returns true if this instance is connected to a physical Manta, false
 *          if not
 *
 ****************************************************************************/
bool MantaUSB::IsConnected(void)
{
   return DeviceHandle != NULL;
}

/************************************************************************//**
 * \brief Connects this instance to a Manta
 * \param connectionSerial The serial number of the manta to search for.
 *
 * If connectionSerial is left out or given as 0 then any connected Manta will
 * match. If a serial number is given then libmanta will attempt to connect to
 * that Manta. If no matching manta is found then Connect will throw a
 * MantaNotFoundException. If no exception is thrown then the connection can be
 * assumed to have been successful.
 *
 ****************************************************************************/
void MantaUSB::Connect(int connectionSerial)
{
#define SERIAL_STRING_SIZE 32
   wchar_t serialString[SERIAL_STRING_SIZE];

   if(IsConnected())
   {
      return;
   }

   DebugPrint("%s-%d: Attempting to Connect to Manta %d...",
         __FILE__, __LINE__, connectionSerial);
   if(connectionSerial)
   {
      swprintf(serialString, SERIAL_STRING_SIZE, L"%d", connectionSerial);
      DeviceHandle = hid_open(VendorID, ProductID, serialString);
   }
   else
   {
      DeviceHandle = hid_open(VendorID, ProductID, NULL);
   }
   if(NULL == DeviceHandle)
      throw(MantaNotFoundException());
   hid_get_serial_number_string(DeviceHandle, serialString, SERIAL_STRING_SIZE);
   SerialNumber = wcstol(serialString, NULL, 10);
   hid_set_nonblocking(DeviceHandle, 1);
}

/************************************************************************//**
 * \brief Disconnects this instance from an attached Manta
 ****************************************************************************/
void MantaUSB::Disconnect(void)
{
   if(! IsConnected())
   {
      return;
   }

   DebugPrint("%s-%d: Manta %d Disconnecting...", __FILE__, __LINE__, GetSerialNumber());
   hid_close(DeviceHandle);
   DeviceHandle = NULL;
}

/************************************************************************//**
 * \brief   Services USB communciation with the Manta
 *
 * HandleEvents should be called periodically to poll all connected Mantas for
 * incoming USB frames as well as to send any messages that have been queued
 * up with WriteFrame(). It should be called at least once every 6ms, but you
 * may get improved results polling as fast as every 1ms if your application
 * supports it.
 *
 * Note: Because WriteFrame() accesses the same message queue that HandleEvents()
 * does, they should be protected from each other by a mutex on the application
 * level if they're being called from parallel threads.
 ****************************************************************************/
void MantaUSB::HandleEvents(void)
{
   list<MantaUSB *>::iterator i = mantaList.begin();
   /* read from each manta and trigger any events */
   while(mantaList.end() != i)
   {
      MantaUSB *current = *i;
      if(current->IsConnected())
      {
         int bytesRead;
         int8_t inFrame[InPacketLen];

         bytesRead = hid_read(current->DeviceHandle,
               reinterpret_cast<uint8_t *>(inFrame), InPacketLen);
         if(bytesRead < 0)
         {
            current->DebugPrint("%s-%d: Read error on Manta %d",
                  __FILE__, __LINE__, current->GetSerialNumber());
            throw(MantaCommunicationException(current));
         }
         else if(bytesRead)
         {
            current->FrameReceived(inFrame);
         }
      }
      ++i;
   }

   /* pop one item off the transmit queue and send down to its target */
   if(! txQueue.empty())
   {
      int bytesWritten;
      MantaTxQueueEntry *txMessage = txQueue.front();
      txQueue.pop_front();
      bytesWritten = hid_write(txMessage->TargetManta->DeviceHandle,
            txMessage->OutFrame, OutPacketLen + 1);
      txMessage->TargetManta->DebugPrint("%s-%d: Frame Written to Manta %d",
            __FILE__, __LINE__, txMessage->TargetManta->GetSerialNumber());
      for(int i = 0; i < 16; i += 8)
      {
         uint8_t *frame = txMessage->OutFrame + 1;
         txMessage->TargetManta->DebugPrint("\t\t%x %x %x %x %x %x %x %x",
               frame[i], frame[i+1], frame[i+2], frame[i+3], frame[i+4],
               frame[i+5], frame[i+6], frame[i+7]);
      }
      delete txMessage;
      if(bytesWritten < 0)
      {
         txMessage->TargetManta->DebugPrint("%s-%d: Write error on Manta %d",
               __FILE__, __LINE__, txMessage->TargetManta->GetSerialNumber());
         throw(MantaCommunicationException(txMessage->TargetManta));
      }
   }
}

/************************************************************************//**
 * \brief   Queries the serial number of the attached Manta
 *
 * \returns The serial number as an int
 ****************************************************************************/
int MantaUSB::GetSerialNumber(void)
{
   /* TODO: get serial number */
   return SerialNumber;
}

MantaUSB::MantaTxQueueEntry *MantaUSB::GetQueuedTxMessage()
{
   list<MantaTxQueueEntry *>::iterator i = txQueue.begin();
   /* look for the first queued message matching this manta */
   while(txQueue.end() != i)
   {
      if((*i)->TargetManta == this)
      {
         return *i;
      }
      ++i;
   }
   return NULL;
}

/* define static class members */
list<MantaUSB *> MantaUSB::mantaList;
list<MantaUSB::MantaTxQueueEntry *> MantaUSB::txQueue;
