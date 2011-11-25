#include <libusb-1.0/libusb.h>
#include <cassert>
#include "MantaUSB.h"
#include "MantaExceptions.h"
#include <cstdlib>

/* declare the C-compatible callback functions */
void MantaOutTransferCompleteHandler(struct libusb_transfer *transfer);
void MantaInTransferCompleteHandler(struct libusb_transfer *transfer);

MantaUSB::MantaUSB(void) :
   SerialNumber(0),
   DeviceHandle(NULL),
   CurrentOutTransfer(NULL),
   CurrentInTransfer(NULL),
   OutTransferQueued(false),
   TransferError(false)
{
   MantaIndex = DeviceCount;
   if(DeviceCount++ == 0)
   {
      if(LIBUSB_SUCCESS != libusb_init(&LibusbContext))
      {
         throw(LibusbInitException());
      }
   }

   DebugPrint("%s-%d: Manta %d initialized", __FILE__, __LINE__, MantaIndex);
}

MantaUSB::~MantaUSB(void)
{
   Disconnect();
   if(0 == (--DeviceCount))
   {
      libusb_exit(LibusbContext);
   }
}

void MantaUSB::WriteFrame(uint8_t *frame)
{
   int status;
   if(NULL == DeviceHandle)
   {
      throw(MantaNotConnectedException());
   }
   /* if a transfer is in progress, just updated the queued
    * transfer frame */
   /* TODO: need to wrap this in a mutex */
   if(CurrentOutTransfer)
   {
      for(int i = 0; i < OutPacketLen; ++i)
      {
         QueuedOutFrame[i] = frame[i];
      }
      OutTransferQueued = true;
   }
   else
   {
      /* no transfer in progress, start a new one */
      CurrentOutTransfer = libusb_alloc_transfer(0);
      libusb_fill_interrupt_transfer(CurrentOutTransfer, DeviceHandle,
            EndpointOut, frame, OutPacketLen, MantaOutTransferCompleteHandler,
            this, Timeout);
      status = libusb_submit_transfer(CurrentOutTransfer);
      if(LIBUSB_SUCCESS != status)
      {
         DebugPrint("%s-%d: (WriteFrame) libusb_submit_transfer failed with status %d on Manta %d", __FILE__, __LINE__, status, MantaIndex);
         libusb_free_transfer(CurrentOutTransfer);
         CurrentOutTransfer = NULL;
         Disconnect();
         throw(MantaCommunicationException());
      }
   }
}

bool MantaUSB::IsConnected(void)
{
   return DeviceHandle != NULL;
}

void MantaUSB::Connect(int connectionSerial)
{
   if(! IsConnected())
   {
      DebugPrint("%s-%d: Manta %d Connecting...", __FILE__, __LINE__, MantaIndex);
      DeviceHandle = GetMantaDeviceHandle(&connectionSerial);
      if(NULL == DeviceHandle)
         throw(MantaNotFoundException());
      SerialNumber = connectionSerial;
#if 0
      if(LIBUSB_SUCCESS != libusb_set_configuration(DeviceHandle, 1))
      {
         Disconnect();
         throw(MantaOpenException());
      }
#endif

      /* start the first read transfer */
      CurrentInTransfer = libusb_alloc_transfer(0);
      BeginReadTransfer();
   }
}

void MantaUSB::Disconnect(void)
{
   if(IsConnected())
   {
      DebugPrint("%s-%d: Manta %d Disconnecting...", __FILE__, __LINE__, MantaIndex);
      CancelEvents();
      libusb_release_interface(DeviceHandle, 0);
      libusb_reset_device(DeviceHandle);
      libusb_close(DeviceHandle);
      DeviceHandle = NULL;
   }
}

/* TODO: this should be a static method because it handles events for all
 * connected mantas. Also, the exceptions thrown should indicate which
 * connected manta object caused the issue, so the application can handle
 * it accordingly */
void MantaUSB::HandleEvents(void)
{
   if(! IsConnected())
   {
      throw(MantaNotConnectedException());
   }
   if(TransferError)
   {
      DebugPrint("%s-%d: Transfer error caught in HandleEvents on Manta %d", __FILE__, __LINE__, MantaIndex);
      TransferError = false;
      Disconnect();
      throw(MantaCommunicationException());
   }
   libusb_handle_events(LibusbContext);
}

bool MantaUSB::IsTransmitting(void)
{
   return (CurrentOutTransfer != NULL);
}

int MantaUSB::GetSerialNumber(void)
{
   /* TODO: get serial number */
   return SerialNumber;
}

/* these Begin*Transfer functions shouldn't throw exceptions directly
 * because they get called from the TransferComplete callbacks, which
 * in turn are called by libusb and don't get to clean up properly
 * if an exception is thrown */
void MantaUSB::BeginReadTransfer(void)
{
   int status;
   libusb_fill_interrupt_transfer(CurrentInTransfer, DeviceHandle, EndpointIn,
         ReceivedFrame, InPacketLen, MantaInTransferCompleteHandler,
         this, Timeout);
   status = libusb_submit_transfer(CurrentInTransfer);
   if(LIBUSB_SUCCESS != status)
   {
      DebugPrint("%s-%d: (BeginReadTransfer) libusb_submit_transfer failed with status %d on Manta %d", __FILE__, __LINE__, status, MantaIndex);
      libusb_free_transfer(CurrentInTransfer);
      CurrentInTransfer = NULL;
      TransferError = true;
   }
}

void MantaUSB::BeginQueuedTransfer(void)
{
   int status;
   libusb_fill_interrupt_transfer(CurrentOutTransfer, DeviceHandle, EndpointOut,
         QueuedOutFrame, OutPacketLen, MantaOutTransferCompleteHandler, this,
         Timeout);
   status = libusb_submit_transfer(CurrentOutTransfer);
   if(LIBUSB_SUCCESS != status)
   {
      DebugPrint("%s-%d: (BeginQueuedTransfer) libusb_submit_transfer failed with status %d on Manta %d", __FILE__, __LINE__, status, MantaIndex);
      libusb_free_transfer(CurrentOutTransfer);
      CurrentOutTransfer = NULL;
      TransferError = true;
   }
}

void MantaUSB::CancelEvents(void)
{
   /* wait until any pending transfers are complete */
   if(CurrentOutTransfer)
   {
      libusb_cancel_transfer(CurrentOutTransfer);
   }
   if(CurrentInTransfer)
   {
      libusb_cancel_transfer(CurrentInTransfer);
   }
   while(CurrentOutTransfer || CurrentInTransfer)
   {
      HandleEvents();
   }
}

/* takes a pointer to a serial number and returns a matching device handle.
 * If the serial number is 0 then the first matching manta is returned, and the
 * serial number is set to the serial number of the device. If the serial number
 * is nonzero then only a matching manta will be returned. If serial is NULL then
 * NULL is always returned as the handle */
libusb_device_handle *MantaUSB::GetMantaDeviceHandle(int *serial)
{
   int status;
   libusb_device **devList;
   int devListSize;
   libusb_device *iter;
   libusb_device_handle *handle;
   struct libusb_device_descriptor desc;
   unsigned char serialString[10];
   int currentDeviceSerial;
   int i;

   if(NULL == serial)
   {
      return NULL;
   }

   devListSize = libusb_get_device_list(LibusbContext, &devList);

   for(i = 0; i < devListSize; ++i)
   {
      iter = devList[i];
      status = libusb_open(iter, &handle);
      if(LIBUSB_SUCCESS == status)
      {
         status = libusb_get_device_descriptor(iter, &desc);
         if(desc.idVendor == VendorID && desc.idProduct == ProductID)
         {
            libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, serialString, sizeof(serialString));
            /* yes, reinterpret_cast is usually a bad idea, but it seems to be
             * the best way to pass a char* to a function expecting a const char* */
            currentDeviceSerial= atoi(reinterpret_cast<const char *>(serialString));
            if(*serial == 0 || currentDeviceSerial == *serial)
            {
               *serial = currentDeviceSerial;
               if(1 == libusb_kernel_driver_active(handle, 0))
               {
                  libusb_detach_kernel_driver(handle, 0);
               }
               //libusb_claim_interface(handle, 0);
               libusb_free_device_list(devList, 1);
               return handle;
            }
         }
         else
         {
            libusb_close(handle);
         }
      }
   }

   libusb_free_device_list(devList, 1);
   return NULL;
}
/* define static class members */
int MantaUSB::DeviceCount = 0;
libusb_context *MantaUSB::LibusbContext;

/* define callback functions outside of the class */
void MantaInTransferCompleteHandler(struct libusb_transfer *transfer)
{
   MantaUSB *device = static_cast<MantaUSB *>(transfer->user_data); 

   assert(transfer == device->CurrentInTransfer);
   if(LIBUSB_TRANSFER_COMPLETED == transfer->status)
   {
      device->FrameReceived(
            reinterpret_cast<int8_t *>(transfer->buffer));
      device->BeginReadTransfer();
   }
   else
   {
      device->DebugPrint("%s-%d: InTransferCompleteHandler failed with status %d on Manta %d", __FILE__, __LINE__, transfer->status, device->MantaIndex);
      libusb_free_transfer(transfer);
      device->CurrentInTransfer = NULL;
      if(LIBUSB_TRANSFER_CANCELLED != transfer->status)
      {
         device->TransferError = true;
      }
   }
}

void MantaOutTransferCompleteHandler(struct libusb_transfer *transfer)
{
   MantaUSB *device = static_cast<MantaUSB *>(transfer->user_data); 

   assert(transfer == device->CurrentOutTransfer);
   if(LIBUSB_TRANSFER_COMPLETED == transfer->status)
   {
      if(device->OutTransferQueued)
      {
         device->OutTransferQueued = false;
         device->BeginQueuedTransfer();
      }
      else
      {
         libusb_free_transfer(transfer);
         device->CurrentOutTransfer = NULL;
      }
   }
   else
   {
      device->DebugPrint("%s-%d: OutTransferCompleteHandler failed with status %d on Manta %d", __FILE__, __LINE__, transfer->status, device->MantaIndex);
      libusb_free_transfer(transfer);
      device->CurrentOutTransfer = NULL;
      if(LIBUSB_TRANSFER_CANCELLED != transfer->status)
      {
         device->TransferError = true;
      }
   }
}
