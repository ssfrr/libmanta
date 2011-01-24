#include "MantaUSB.h"

class Manta : public MantaUSB
{
   public:
      enum LEDColor {
         Amber,
         Red
      };
      enum LEDControlType {
         PadAndButton,
         Slider,
         Button
      };
      typedef uint8_t LEDFrame[6];

      Manta(void);
      void SetLED(LEDColor color, int column, int row, bool enabled);
      void SetLEDRow(LEDColor color, int row, uint8_t mask);
      void SetLEDColumn(LEDColor color, int column, uint8_t mask);
      void SetLEDFrame(LEDColor color, uint8_t mask[]);
      void SetSliderLEDs(int id, uint8_t mask);
      void SetButtonLEDs(LEDColor color, int id, bool enabled);
      void Recalibrate(void);
      void SetLEDControl(LEDControlType control, bool state);
      void SetTurboMode(bool Enabled);
      void SetRawMode(bool Enabled);
      void SetHiResMode(bool Enabled);

   protected:
      /* declare callbacks */
      virtual void PadEvent(int id, int value) = 0;
      virtual void SliderEvent(int id, int value) = 0;
      virtual void ButtonEvent(int id, int value) = 0;
      /* declare callbacks implemented by this class */
      virtual void FrameReceived(int8_t *frame);
      
   private:
      static uint8_t byteReverse(uint8_t inByte);
      int8_t LastInReport[InPacketLen];
      uint8_t CurrentOutReport[OutPacketLen];
};

