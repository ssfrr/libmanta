#include "MantaUSB.h"

class Manta : public MantaUSB
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

      Manta(void);
      void SetPadLED(LEDState state, int ledID);
      void SetPadLEDRow(LEDState state, int row, uint8_t mask);
      void SetPadLEDColumn(LEDState state, int column, uint8_t mask);
      void SetPadLEDFrame(LEDState state, uint8_t mask[]);
      void SetSliderLED(LEDState state, int id, uint8_t mask);
      void SetButtonLED(LEDState state, int id);
      void Recalibrate(void);
      void SetLEDControl(LEDControlType control, bool state);
      void SetTurboMode(bool Enabled);
      void SetRawMode(bool Enabled);

   protected:
      /* declare callbacks to be implemented by subclasses */
      virtual void PadEvent(int id, int value) = 0;
      virtual void SliderEvent(int id, int value) = 0;
      virtual void ButtonEvent(int id, int value) = 0;
      virtual void VelocityEvent(int id, int velocity) = 0;
      /* declare superclass callbacks implemented by this class */
      virtual void FrameReceived(int8_t *frame);
      
   private:
      static uint8_t byteReverse(uint8_t inByte);
      static int CalculateVelocity(int firstValue, int secondValue);
      static const int AmberIndex = 0;
      static const int RedIndex = 10;
      static const int SliderIndex = 7;
      static const int ButtonIndex = 6;
      static const int ConfigIndex = 9;
      int8_t LastInReport[InPacketLen];
      uint8_t CurrentOutReport[OutPacketLen];
      bool VelocityWaiting[49];
};

