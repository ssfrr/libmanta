#include "MantaUSB.h"
#include "MantaExceptions.h"

MantaUSB::MantaUSB(int serialNumber)
{
   if(DeviceCount++ == 0)
   {
      libusb_init(&LibusbContext);
   }

   OutPacketLen = 16;
   InPacketLen = 64;
   DeviceHandle = NULL;
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
   if(LIBUSB_SUCCESS != libusb_interrupt_transfer(DeviceHandle, EndpointOut, reinterpret_cast<unsigned char *>(frame), OutPacketLen,
         &transferred, Timeout))
      throw(MantaCommunicationException());
}

void MantaUSB::ReadFrame(void *frame)
{
   int transferred;
   if(LIBUSB_SUCCESS != libusb_interrupt_transfer(DeviceHandle, EndpointIn, reinterpret_cast<unsigned char *>(frame), InPacketLen,
         &transferred, Timeout))
      throw(MantaCommunicationException());
}

bool MantaUSB::IsConnected(void)
{
   return DeviceHandle != NULL;
}

void MantaUSB::Connect(void)
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
}

/* define static members */
int MantaUSB::DeviceCount = 0;
libusb_context *MantaUSB::LibusbContext;
