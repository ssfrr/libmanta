#ifndef _MANTASERVER_H
#define _MANTASERVER_H

/************************************************************************//**
 * \class MantaServer
 * \brief Interface defining all the Messages that can be sent to a Manta
 *
 * The MantaServer virtual class defines all the Messages that the Manta
 * understands, as well as the data structures used as arguments. If you
 * need a pointer to a Manta in your code, you can make it more general by
 * using a MantaServer pointer instead of a pointer to your specific subclass.
 ****************************************************************************/
class MantaServer
{
   public:

   enum LEDState {
      Off,
      Amber,
      Red,
      All, // only used in SetPadLEDFrame
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
