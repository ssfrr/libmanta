#ifndef _MANTA_H
#define _MANTA_H

#include "MantaUSB.h"
#include "MantaClient.h"
#include "MantaServer.h"

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

   private:
      /* declare superclass callback implemented by this class */
      virtual void FrameReceived(int8_t *frame);

      static uint8_t byteReverse(uint8_t inByte);
      static int CalculateVelocity(int firstValue, int secondValue);
      static int ScaleSensorValue(int rawValue, int index);
      static const int AmberIndex = 0;
      static const int RedIndex = 10;
      static const int SliderIndex = 7;
      static const int ButtonIndex = 6;
      static const int ConfigIndex = 9;
      static const int MaxSensorValues[53];

      uint8_t LastInReport[InPacketLen];
      uint8_t CurrentOutReport[OutPacketLen];
      bool VelocityWaiting[53];
      int MaximumPadID;
      int MaximumPadValue;
      /* output modes */
      bool CentroidEnabled;
      bool MaximumEnabled;
      bool PadFrameEnabled;
};

#endif // _MANTA_H
