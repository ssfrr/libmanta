#ifndef _MANTAUSB_H
#define _MANTAUSB_H


#include <stdint.h>
#include <list>

using namespace std;

/* forward-declare hidapi types so we don't need to include the
 * whole header file */

typedef struct hid_device_ hid_device;

/************************************************************************//**
 * \class MantaUSB
 * \brief Superclass that handles the low-level USB communication with the
 * Manta
 *
 * The MantaUSB class handles the low-level USB communication with the Manta
 * over hidAPI, a cross-platform HID library.
 *
 * The public methods of this class are meant to be used by applciations using
 * libmanta to manage the connection to a physical Manta, as well as servicing
 * the low-level USB drivers by periodically calling the HandleEvents()
 * function.
 ****************************************************************************/
class MantaUSB
{
   public:
      MantaUSB(void);
      virtual ~MantaUSB(void);
      void WriteFrame(uint8_t *frame, bool forceQueued);
      bool IsConnected(void);
      void Connect(int connectionSerial = 0);
      void Disconnect();
      int GetSerialNumber(void);
      int GetHardwareVersion(void);
      bool MessageQueued(void);

      static void HandleEvents(void);

   protected:
      virtual void FrameReceived(int8_t *frame) = 0;
      virtual void DebugPrint(const char *fmt, ...) {}
      static const int OutPacketLen = 16;
      static const int InPacketLen = 64;
      int SerialNumber;
      int MantaIndex;

   private:
      struct MantaTxQueueEntry
      {
         MantaUSB *TargetManta;
         uint8_t OutFrame[17];
      };

      MantaTxQueueEntry *GetQueuedTxMessage();

      static const int Interface = 0;
      static const int EndpointIn = 0x81; /* endpoint 0x81 address for IN */
      static const int EndpointOut = 0x02; /* endpoint 1 address for OUT */
      static const int Timeout = 5000; /* timeout in ms */
      static const int VendorID = 0x2424;
      static const int ProductID = 0x2424;

      hid_device *DeviceHandle;

      static list<MantaUSB *> mantaList;
      static list<MantaTxQueueEntry *> txQueue;
};

#endif // _MANTAUSB_H
