#ifndef _MANTA_H
#define _MANTA_H

#include "MantaUSB.h"
#include "MantaClient.h"
#include "MantaServer.h"

/************************************************************************//**
 * \class Manta
 * \brief Superclass that provides an interface to the Manta
 *
 * The Manta class is intended to use as a base class within your application.
 * In order to use libmanta, subclass this and define implementations for any
 * callback functions that you would like your application to be notified of.
 * The event functions are declared in the MantaClient interface.
 *
 * Creating an instance of the Manta class (or a subclass) will not initiate
 * a connection to any plugged-in mantas. Before you can start communicating
 * with the Manta you must call its Connect() method to connect to a physical
 * Manta over USB.
 *
 * Once connected, your application should periodically call the static method
 * HandleEvents(). This will take care of servicing the low-level USB
 * communication for all connected Mantas.
 ****************************************************************************/
class Manta :
   public MantaUSB,
   public MantaClient,
   public MantaServer
{
   public:
      Manta(void);

      /* MantaServer messages implemented here */
      virtual void SetPadLED(LEDState state, int ledID);
      virtual void SetPadLEDRow(LEDState state, int row, uint8_t mask);
      virtual void SetPadLEDColumn(LEDState state, int column, uint8_t mask);
      virtual void SetPadLEDFrame(LEDState state, uint8_t mask[]);
      virtual void SetSliderLED(LEDState state, int id, uint8_t mask);
      virtual void SetButtonLED(LEDState state, int id);
      virtual void ResendLEDState(void);
      virtual void ClearPadAndButtonLEDs(void);
      virtual void ClearButtonLEDs(void);
      virtual void Recalibrate(void);
      virtual void SetLEDControl(LEDControlType control, bool state);
      virtual void SetTurboMode(bool Enabled);
      virtual void SetRawMode(bool Enabled);
      virtual void SetMaxSensorValues(int *values);

   private:
      /* declare superclass callback implemented by this class */
      virtual void FrameReceived(int8_t *frame);
      int ScaleSensorValue(int rawValue, int index);

      static uint8_t byteReverse(uint8_t inByte);
      static int CalculateVelocity(int firstValue, int secondValue);
      static const int AmberIndex = 0;
      static const int RedIndex = 10;
      static const int SliderIndex = 7;
      static const int ButtonIndex = 6;
      static const int ConfigIndex = 9;
      static const int AverageMaxSensorValues[53];

      int MaxSensorValues[53];
      uint8_t LastInReport[InPacketLen];
      uint8_t CurrentOutReport[OutPacketLen];
      bool VelocityWaiting[53];
      /* output modes */
      bool CentroidEnabled;
      bool MaximumEnabled;
      bool PadFrameEnabled;
};

#endif // _MANTA_H
