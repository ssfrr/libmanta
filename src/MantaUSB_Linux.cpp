#include "MantaUSB.h"
#include <iostream>

MantaUSB::MantaUSB(int serialNumber)
{
   if(DeviceCount++ == 0)
   {
      if(LIBUSB_SUCCESS == libusb_init(&LibusbContext))
         std::cout << "Libusb Init Successful" << std::endl;
      else
         std::cout << "Libusb Init Failed" << std::endl;
   }

   OutPacketLen = 16;
   InPacketLen = 64;

   DeviceHandle = libusb_open_device_with_vid_pid(LibusbContext, VendorID, ProductID);
   if(NULL != DeviceHandle)
      std::cout << "Device Open Successful" << std::endl;
   else
      std::cout << "Device Open Failed" << std::endl;
   if(LIBUSB_SUCCESS == libusb_detach_kernel_driver(DeviceHandle, 0))
      std::cout << "Kernel Driver Detach Successful" << std::endl;
   else
      std::cout << "Kernel Driver Detach Failed" << std::endl;
   if(LIBUSB_SUCCESS == libusb_set_configuration(DeviceHandle, 1))
      std::cout << "Set Configuration Successful" << std::endl;
   else
      std::cout << "Set Configuration Failed" << std::endl;
   if(LIBUSB_SUCCESS == libusb_claim_interface(DeviceHandle, 0))
      std::cout << "Claim Interface Successful" << std::endl;
   else
      std::cout << "Claim Interface Failed" << std::endl;
   libusb_set_debug(LibusbContext,3);
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
   libusb_interrupt_transfer(DeviceHandle, EndpointOut, reinterpret_cast<unsigned char *>(frame), OutPacketLen,
         &transferred, Timeout);
}

void MantaUSB::ReadFrame(void *frame)
{
   int transferred;
   libusb_interrupt_transfer(DeviceHandle, EndpointIn, reinterpret_cast<unsigned char *>(frame), InPacketLen,
         &transferred, Timeout);
}

bool MantaUSB::IsOpen(void)
{
   return DeviceHandle != NULL;
}

/* define static members */
int MantaUSB::DeviceCount = 0;
libusb_context *MantaUSB::LibusbContext;
