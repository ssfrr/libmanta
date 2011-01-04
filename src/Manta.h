#include "MantaUSB.h"

class Manta
{
   public:
      enum LEDColor {
         Amber,
         Red
      };
      enum MantaStatus {
         Success,
         NotConnected,
         InvalidArgument
      };
      enum LEDControlType {
         PadAndButton,
         Slider,
         Button
      };
      typedef uint8_t LEDFrame[6];

      Manta(int serialNumber = 0);
      MantaStatus StartPoll(void);
      MantaStatus StopPoll(void);
      MantaStatus SetLED(LEDColor color, int column, int row, bool enabled);
      MantaStatus SetLEDRow(LEDColor color, int row, uint8_t mask);
      MantaStatus SetLEDColumn(LEDColor color, int column, uint8_t mask);
      MantaStatus SetLEDFrame(LEDColor color, uint8_t mask[]);
      MantaStatus SetSliderLEDs(int id, uint8_t mask);
      MantaStatus SetButtonLEDs(LEDColor color, int id, bool enabled);
      MantaStatus Recalibrate(void);
      MantaStatus SetLEDControl(LEDControlType control, bool state);
      MantaStatus SetTurboMode(bool Enabled);
      MantaStatus SetRawMode(bool Enabled);
      MantaStatus SetHiResMode(bool Enabled);

   private:
      uint8_t byteReverse(uint8_t inByte);
      virtual void PadEvent(int id, int value) = 0;
      virtual void SliderEvent(int id, int value) = 0;
      virtual void ButtonEvent(int id, int value) = 0;
      
      MantaUSB Dev;
      int8_t LastInReport[64];
      uint8_t CurrentOutReport[16];
      volatile bool polling;
};

