#include <libusb-1.0/libusb.h>
#include <cassert>
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
   CurrentOutTransfer = NULL;
   CurrentInTransfer = NULL;
   OutTransferQueued = false;
   TransferError = false;
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
   int transferred;
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
      if(LIBUSB_SUCCESS != libusb_submit_transfer(CurrentOutTransfer))
      {
         libusb_free_transfer(CurrentOutTransfer);
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
   CurrentInTransfer = libusb_alloc_transfer(0);
   BeginReadTransfer();
}

void MantaUSB::Disconnect(void)
{
   if(! IsConnected())
   {
      CancelEvents();
      libusb_release_interface(DeviceHandle, 0);
      libusb_reset_device(DeviceHandle);
      libusb_close(DeviceHandle);
      DeviceHandle = NULL;
   }
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
   return (CurrentOutTransfer != NULL);
}

/* these Begin*Transfer functions shouldn't throw exceptions directly
 * because they get called from the TransferComplete callbacks, which
 * in turn are called by libusb and don't get to clean up properly
 * if an exception is thrown */
void MantaUSB::BeginReadTransfer(void)
{
   libusb_fill_interrupt_transfer(CurrentInTransfer, DeviceHandle, EndpointIn,
         ReceivedFrame, InPacketLen, MantaInTransferCompleteHandler,
         this, Timeout);
   if(LIBUSB_SUCCESS != libusb_submit_transfer(CurrentInTransfer))
   {
      libusb_free_transfer(CurrentInTransfer);
      CurrentInTransfer = NULL;
      TransferError = true;
   }
}

void MantaUSB::BeginQueuedTransfer(void)
{
   libusb_fill_interrupt_transfer(CurrentOutTransfer, DeviceHandle, EndpointOut,
         QueuedOutFrame, OutPacketLen, MantaOutTransferCompleteHandler, this,
         Timeout);
   if(LIBUSB_SUCCESS != libusb_submit_transfer(CurrentOutTransfer))
   {
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

/* define static class members */
int MantaUSB::DeviceCount = 0;
libusb_context *MantaUSB::LibusbContext;

/* define callback functions outside of the class */
void MantaInTransferCompleteHandler(struct libusb_transfer *transfer)
{
   assert(transfer ==
         static_cast<MantaUSB *>(transfer->user_data)->CurrentInTransfer);
   if(LIBUSB_TRANSFER_COMPLETED == transfer->status)
   {
      static_cast<MantaUSB *>(transfer->user_data)->FrameReceived(
            reinterpret_cast<int8_t *>(transfer->buffer));
      static_cast<MantaUSB *>(transfer->user_data)->BeginReadTransfer();
   }
   else
   {
      libusb_free_transfer(transfer);
      static_cast<MantaUSB *>(transfer->user_data)->CurrentInTransfer = NULL;
      if(LIBUSB_TRANSFER_CANCELLED != transfer->status)
      {
         static_cast<MantaUSB *>(transfer->user_data)->TransferError = true;
      }
   }
}

void MantaOutTransferCompleteHandler(struct libusb_transfer *transfer)
{
   assert(transfer ==
         static_cast<MantaUSB *>(transfer->user_data)->CurrentOutTransfer);
   if(LIBUSB_TRANSFER_COMPLETED == transfer->status)
   {
      if(static_cast<MantaUSB *>(transfer->user_data)->OutTransferQueued)
      {
         static_cast<MantaUSB *>(transfer->user_data)->OutTransferQueued = false;
         static_cast<MantaUSB *>(transfer->user_data)->BeginQueuedTransfer();
      }
      else
      {
         libusb_free_transfer(transfer);
         static_cast<MantaUSB *>(transfer->user_data)->CurrentOutTransfer = NULL;
      }
   }
   else
   {
      libusb_free_transfer(transfer);
      static_cast<MantaUSB *>(transfer->user_data)->CurrentOutTransfer = NULL;
      if(LIBUSB_TRANSFER_CANCELLED != transfer->status)
      {
         static_cast<MantaUSB *>(transfer->user_data)->TransferError = true;
      }
   }
}
