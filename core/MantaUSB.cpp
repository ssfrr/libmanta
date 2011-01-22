#include "MantaUSB.h"
#include "MantaExceptions.h"

MantaUSB::MantaUSB(void)
{
   if(DeviceCount++ == 0)
   {
      libusb_init(&LibusbContext);
   }

   DeviceHandle = NULL;
   DeviceWriteTransfer = NULL;
   OutTransferInProgress = false;
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

void MantaUSB::WriteFrame(void *frame)
{
   int transferred;
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
            EndpointOut, frame, OutPacketLen, OutTransferCompleteHandler,
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

void MantaUSB::Connect(int serialNumber = 0)
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
   libusb_fill_interrupt_transfer(transfer, DeviceHandle,
         EndpointIn, ReceivedFrame, InPacketLen, InTransferCompleteHandler,
         this, Timeout);
   if(LIBUSB_SUCCESS != libusb_submit_transfer(transfer))
   {
      libusb_free_transfer(transfer);
      throw(MantaCommunicationException());
   }
}

void MantaUSB::PollForEvents(void)
{
   libusb_handle_events(LibusbContext);
}

/* define static class members */
int MantaUSB::DeviceCount = 0;
libusb_context *MantaUSB::LibusbContext;

/* define callback functions outside of the class */
static void InTransferCompleteHandler(struct libusb_transfer *transfer)
{
   if(LIBUSB_TRANSFER_COMPLETED != transfer->status)
   {
      libusb_free_transfer(transfer);
      throw(MantaCommunicationException());
   }

   static_cast<MantaUSB *>(transfer->user_data)->FrameReceived(transfer->buffer);

   libusb_fill_interrupt_transfer(transfer, DeviceHandle,
         EndpointIn, ReceivedFrame, OutPacketLen, OutTransferCompleteHandler,
         transfer->user_data, Timeout);
   if(LIBUSB_SUCCESS != libusb_submit_transfer(transfer))
   {
      libusb_free_transfer(transfer);
      throw(MantaCommunicationException());
   }
}

static void OutTransferCompleteHandler(struct libusb_transfer *transfer)
{
   libusb_free_transfer(transfer);
   if(OutTransferQueued)
   {
      struct libusb_transfer *transfer = libusb_alloc_transfer(0);
      OutTransferQueued = 0;
      libusb_fill_interrupt_transfer(transfer, DeviceHandle,
            EndpointOut, QueuedOutFrame, OutPacketLen, OutTransferCompleteHandler,
            NULL, Timeout);
      if(LIBUSB_SUCCESS != libusb_submit_transfer(transfer))
      {
         libusb_free_transfer(transfer);
         throw(MantaCommunicationException());
      }
   }
   else
   {
      OutTransferInProgress = false;
   }
}
