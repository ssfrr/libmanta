#include <libusb-1.0/libusb.h>
#include "MantaUSB.h"
#include "MantaExceptions.h"

/* declare the C-compatible callback functions */
void MantaOutTransferCompleteHandler(struct libusb_transfer *transfer);
void MantaInTransferCompleteHandler(struct libusb_transfer *transfer);

MantaUSB::MantaUSB(void)
{
   if(DeviceCount++ == 0)
   {
      if(LIBUSB_SUCCESS != libusb_init(&LibusbContext))
      {
         throw(LibusbInitException());
      }
   }

   DeviceHandle = NULL;
   OutTransferInProgress = false;
   OutTransferQueued = false;
   TransferError = false;
}

MantaUSB::~MantaUSB(void)
{
   libusb_release_interface(DeviceHandle, 0);
   libusb_reset_device(DeviceHandle);
   libusb_close(DeviceHandle);
   if(0 == (--DeviceCount))
   {
      libusb_exit(LibusbContext);
   }
}

void MantaUSB::WriteFrame(uint8_t *frame)
{
   int transferred;
   if(NULL == DeviceHandle)
   {
      throw(MantaNotConnectedException());
   }
   /* if a transfer is in progress, just updated the queued
    * transfer frame */
   /* TODO: need to wrap this in a mutex */
   if(OutTransferInProgress)
   {
      for(int i = 0; i < OutPacketLen; ++i)
      {
         QueuedOutFrame[i] = frame[i];
      }
      OutTransferQueued = true;
   }
   else
   {
      OutTransferInProgress = true;
      /* no transfer in progress, start a new one */
      struct libusb_transfer *transfer = libusb_alloc_transfer(0);
      libusb_fill_interrupt_transfer(transfer, DeviceHandle,
            EndpointOut, frame, OutPacketLen, MantaOutTransferCompleteHandler,
            this, Timeout);
      if(LIBUSB_SUCCESS != libusb_submit_transfer(transfer))
      {
         libusb_free_transfer(transfer);
         throw(MantaCommunicationException());
      }
   }
}

bool MantaUSB::IsConnected(void)
{
   return DeviceHandle != NULL;
}

void MantaUSB::Connect(int serialNumber)
{
   DeviceHandle = libusb_open_device_with_vid_pid(LibusbContext, VendorID, ProductID);
   /*TODO create new types of exceptions */
   if(NULL == DeviceHandle)
      throw(MantaNotFoundException());
   libusb_detach_kernel_driver(DeviceHandle, 0);
   if(LIBUSB_SUCCESS != libusb_set_configuration(DeviceHandle, 1))
      throw(MantaCommunicationException());
   if(LIBUSB_SUCCESS != libusb_claim_interface(DeviceHandle, 0))
      throw(MantaCommunicationException());

   /* start the first read transfer */
   struct libusb_transfer *transfer = libusb_alloc_transfer(0);
   BeginReadTransfer(transfer);
}

void MantaUSB::HandleEvents(void)
{
   if(! IsConnected())
   {
      throw(MantaNotConnectedException());
   }
   if(TransferError)
   {
      TransferError = false;
      throw(MantaCommunicationException());
   }
   libusb_handle_events(LibusbContext);
}

bool MantaUSB::IsTransmitting(void)
{
   return OutTransferInProgress;
}

/* these Begin*Transfer functions shouldn't throw exceptions directly
 * because they get called from the TransferComplete callbacks, which
 * in turn are called by libusb and don't get to clean up properly
 * if an exception is thrown */
void MantaUSB::BeginReadTransfer(libusb_transfer *transfer)
{
   libusb_fill_interrupt_transfer(transfer, DeviceHandle, EndpointIn,
         ReceivedFrame, InPacketLen, MantaInTransferCompleteHandler,
         this, Timeout);
   if(LIBUSB_SUCCESS != libusb_submit_transfer(transfer))
   {
      libusb_free_transfer(transfer);
      TransferError = true;
   }
}

void MantaUSB::BeginQueuedTransfer(struct libusb_transfer *transfer)
{
   libusb_fill_interrupt_transfer(transfer, DeviceHandle, EndpointOut,
         QueuedOutFrame, OutPacketLen, MantaOutTransferCompleteHandler, this,
         Timeout);
   if(LIBUSB_SUCCESS != libusb_submit_transfer(transfer))
   {
      libusb_free_transfer(transfer);
      TransferError = true;
   }
}

/* define static class members */
int MantaUSB::DeviceCount = 0;
libusb_context *MantaUSB::LibusbContext;

/* define callback functions outside of the class */
void MantaInTransferCompleteHandler(struct libusb_transfer *transfer)
{
   if(LIBUSB_TRANSFER_COMPLETED != transfer->status)
   {
      libusb_free_transfer(transfer);
      static_cast<MantaUSB *>(transfer->user_data)->TransferError = true;
   }
   else
   {
      static_cast<MantaUSB *>(transfer->user_data)->FrameReceived(
            reinterpret_cast<int8_t *>(transfer->buffer));
      static_cast<MantaUSB *>(transfer->user_data)->BeginReadTransfer(transfer);
   }
}

void MantaOutTransferCompleteHandler(struct libusb_transfer *transfer)
{
   if(LIBUSB_TRANSFER_COMPLETED != transfer->status)
   {
      libusb_free_transfer(transfer);
      static_cast<MantaUSB *>(transfer->user_data)->TransferError = true;
   }
   else
   {
      if(static_cast<MantaUSB *>(transfer->user_data)->OutTransferQueued)
      {
         static_cast<MantaUSB *>(transfer->user_data)->OutTransferQueued = false;
         static_cast<MantaUSB *>(transfer->user_data)->BeginQueuedTransfer(transfer);
      }
      else
      {
         libusb_free_transfer(transfer);
         static_cast<MantaUSB *>(transfer->user_data)->OutTransferInProgress = false;
      }
   }
}
