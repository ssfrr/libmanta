#include <libusb-1.0/libusb.h>

class MantaUSB
{
   public:
      MantaUSB(int serialNumber = 0);
      ~MantaUSB(void);
      void WriteFrame(void *frame);
      void ReadFrame(void *frame);
      bool IsConnected(void);
      void Connect(void);
   private:
      int OutPacketLen;
      int InPacketLen;
      static const int Interface = 0;
      static const int EndpointIn = 0x81; /* endpoint 0x81 address for IN */
      static const int EndpointOut = 0x02; /* endpoint 1 address for OUT */
      static const int Timeout = 5000; /* timeout in ms */
      static const int VendorID = 0x2424;
      static const int ProductID = 0x2424;

      static int DeviceCount;
      static libusb_context *LibusbContext;

      libusb_device_handle *DeviceHandle;
      int SerialNumber;
};
