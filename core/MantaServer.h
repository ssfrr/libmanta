#ifndef _MANTASERVER_H
#define _MANTASERVER_H

class MantaServer
{
   public:

   enum LEDState {
      Off,
      Amber,
      Red,
   };
   enum LEDControlType {
      PadAndButton,
      Slider,
      Button
   };
   typedef uint8_t LEDFrame[6];

   virtual ~MantaServer() {}
   /* declare callbacks to be implemented by subclasses */
   virtual void SetPadLED(LEDState state, int ledID) = 0;
   virtual void SetPadLEDRow(LEDState state, int row, uint8_t mask) = 0;
   virtual void SetPadLEDColumn(LEDState state, int column, uint8_t mask) = 0;
   virtual void SetPadLEDFrame(LEDState state, uint8_t mask[]) = 0;
   virtual void SetSliderLED(LEDState state, int id, uint8_t mask) = 0;
   virtual void SetButtonLED(LEDState state, int id) = 0;
   virtual void ResendLEDState(void) = 0;
   virtual void ClearPadAndButtonLEDs(void) = 0;
   virtual void ClearButtonLEDs(void) = 0;
   virtual void Recalibrate(void) = 0;
   virtual void SetLEDControl(LEDControlType control, bool state) = 0;
   virtual void SetTurboMode(bool Enabled) = 0;
   virtual void SetRawMode(bool Enabled) = 0;
   virtual void SetMaxSensorValues(int *values) = 0;
};
#endif /* _MANTASERVER_H */
