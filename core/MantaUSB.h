#include <libusb-1.0/libusb.h>

class MantaUSB
{
   public:
      MantaUSB(void);
      ~MantaUSB(void);
      void WriteFrame(void *frame);
      bool IsConnected(void);
      void Connect(int serialNumber = 0);
      void PollForEvents(void);
   private:
      virtual void FrameReceived(void *frame) {}
      static const int OutPacketLen = 16;
      static const int InPacketLen = 64;
      static const int Interface = 0;
      static const int EndpointIn = 0x81; /* endpoint 0x81 address for IN */
      static const int EndpointOut = 0x02; /* endpoint 1 address for OUT */
      static const int Timeout = 5000; /* timeout in ms */
      static const int VendorID = 0x2424;
      static const int ProductID = 0x2424;

      int SerialNumber;
      static int DeviceCount;
      static libusb_context *LibusbContext;
      libusb_device_handle *DeviceHandle;
      volatile bool OutTransferInProgress;
      volatile bool OutTransferQueued;
      volatile uint8_t QueuedOutFrame[OutPacketLen];
      uint8_t ReceivedFrame[InPacketLen];
};
