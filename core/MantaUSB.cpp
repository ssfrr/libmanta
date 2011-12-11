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

/* note: WriteFrame shares the message queue with HandleEvents, so the two
 * functions should be protected by a mutex from simultaneous access */
void MantaUSB::WriteFrame(uint8_t *frame)
{
   int status;
   if(NULL == DeviceHandle)
   {
      throw(MantaNotConnectedException(this));
   }
   MantaTxQueueEntry *queuedMessage = GetQueuedTxMessage();
   if(queuedMessage != NULL)
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

bool MantaUSB::IsConnected(void)
{
   return DeviceHandle != NULL;
}

void MantaUSB::Connect(int connectionSerial)
{
   wchar_t serialString[10];

   if(IsConnected())
   {
      return;
   }

   DebugPrint("%s-%d: Attempting to Connect to Manta %d...",
         __FILE__, __LINE__, connectionSerial);
   if(connectionSerial)
   {
      swprintf(serialString, 10, L"%d", connectionSerial);
      DeviceHandle = hid_open(VendorID, ProductID, serialString);
   }
   else
   {
      DeviceHandle = hid_open(VendorID, ProductID, NULL);
   }
   if(NULL == DeviceHandle)
      throw(MantaNotFoundException());
   hid_get_serial_number_string(DeviceHandle, serialString, 10);
   SerialNumber = wcstol(serialString, NULL, 10);
}

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

/* TODO: the exceptions thrown should indicate which connected manta object
 * caused the issue, so the application can handle it accordingly */
void MantaUSB::HandleEvents(void)
{
   list<MantaUSB *>::iterator i = mantaList.begin();
   /* read from each manta and trigger any events */
   while(mantaList.end() != i)
   {
      MantaUSB *current = *i;
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

int MantaUSB::GetSerialNumber(void)
{
   /* TODO: get serial number */
   return SerialNumber;
}

MantaUSB::MantaTxQueueEntry *MantaUSB::GetQueuedTxMessage()
{
   list<MantaTxQueueEntry *>::iterator i = txQueue.begin();
   /* read from each manta and trigger any events */
   while(txQueue.end() != i)
   {
      if((*i)->TargetManta == this)
      {
         return *i;
      }
   }
   return NULL;
}

/* define static class members */
list<MantaUSB *> MantaUSB::mantaList;
list<MantaUSB::MantaTxQueueEntry *> MantaUSB::txQueue;
