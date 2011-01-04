#include "MantaUSB.h"

class MantaOutFrameBase
{
   public:
      uint8_t PadAmberLEDBitmask[6];
      uint8_t ButtonAmberLEDBitmask;
      uint8_t SliderAmberLEDBitmask[2];
      bool PadAndButtonLEDControlled;
      bool SliderLEDControlled;
      bool TurboMode;
      bool RawMode;
      bool HighResMode;
      bool ButtonLEDControlled;
      bool Recalibrate;
};

class MantaOutFrame2ndEdition : MantaOutFrameBase
{
   public:
      uint8_t PadRedLEDBitmask[6];
};


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
      typedef void (*EventCallback)(int id, int value);

      Manta(int serialNumber = 0);
      MantaStatus StartPoll(void);
      MantaStatus StopPoll(void);
      MantaStatus SetLED(LEDColor color, int column, int row, bool enabled);
      MantaStatus SetLEDRow(LEDColor color, int row, uint8_t mask);
      MantaStatus SetLEDColumn(LEDColor color, int column, uint8_t mask);
      MantaStatus SetLEDFrame(LEDColor color, uint8_t mask[]);
      MantaStatus SetSliderLEDs(int id, uint8_t mask);
      MantaStatus SetButtonLEDs(LEDColor color, int id, bool enabled);
      MantaStatus SetPadCallback(EventCallback callback);
      MantaStatus SetSliderCallback(EventCallback callback);
      MantaStatus SetButtonCallback(EventCallback callback);
      MantaStatus Recalibrate(void);
      MantaStatus SetLEDControl(LEDControlType control, bool state);
      MantaStatus SetTurboMode(bool Enabled);
      MantaStatus SetRawMode(bool Enabled);
      MantaStatus SetHiResMode(bool Enabled);

   private:
      uint8_t byteReverse(uint8_t inByte);
      MantaUSB Dev;
      EventCallback PadCallback;
      EventCallback SliderCallback;
      EventCallback ButtonCallback;
      int8_t LastInReport[64];
      uint8_t CurrentOutReport[16];
      volatile bool polling;
};

