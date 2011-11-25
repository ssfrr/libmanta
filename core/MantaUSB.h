#ifndef _MANTAUSB_H
#define _MANTAUSB_H

/* forward-declare libusb types so we don't need to include the
 * whole header file */

#include <stdint.h>
#include <list>

using namespace std;

typedef struct libusb_transfer libusb_transfer;
typedef struct libusb_context libusb_context;
typedef struct libusb_device_handle libusb_device_handle;

class MantaUSB
{
   public:
      MantaUSB(void);
      virtual ~MantaUSB(void);
      bool IsConnected(void);
      void Connect(int connectionSerial = 0);
      void Disconnect();
      bool IsTransmitting(void);
      int GetSerialNumber(void);

      static void HandleEvents(void);

   protected:
      void WriteFrame(uint8_t *frame);
      virtual void FrameReceived(int8_t *frame) = 0;
      virtual void DebugPrint(const char *fmt, ...) {}
      static const int OutPacketLen = 16;
      static const int InPacketLen = 64;
      int SerialNumber;
      int MantaIndex;

   private:
      static const int Interface = 0;
      static const int EndpointIn = 0x81; /* endpoint 0x81 address for IN */
      static const int EndpointOut = 0x02; /* endpoint 1 address for OUT */
      static const int Timeout = 5000; /* timeout in ms */
      static const int VendorID = 0x2424;
      static const int ProductID = 0x2424;

      void BeginReadTransfer(void);
      void BeginQueuedTransfer(void);
      void CancelEvents(void);
      libusb_device_handle *GetMantaDeviceHandle(int *serial);

      static int DeviceCount;
      static libusb_context *LibusbContext;
      libusb_device_handle *DeviceHandle;
      libusb_transfer *CurrentOutTransfer;
      libusb_transfer *CurrentInTransfer;
      bool OutTransferQueued;
      uint8_t QueuedOutFrame[OutPacketLen];
      uint8_t ReceivedFrame[InPacketLen];
      bool TransferError;

      static list<MantaUSB *> mantaList;

      friend void MantaOutTransferCompleteHandler(struct libusb_transfer *transfer);
      friend void MantaInTransferCompleteHandler(struct libusb_transfer *transfer);
};

#endif // _MANTAUSB_H
