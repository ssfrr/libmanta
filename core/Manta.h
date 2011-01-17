#include "MantaUSB.h"

class Manta
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

      Manta(int serialNumber = 0);
      void Connect(void);
      bool IsConnected(void);
      void StartPoll(void);
      void StopPoll(void);
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

   private:
      void UpdateOutputReport(void);
      static uint8_t byteReverse(uint8_t inByte);
      virtual void PadEvent(int id, int value) {};
      virtual void SliderEvent(int id, int value) {};
      virtual void ButtonEvent(int id, int value) {};
      
      MantaUSB Dev;
      int8_t LastInReport[64];
      uint8_t CurrentOutReport[16];
      volatile bool polling;
      volatile bool OutputReportDirty;
      volatile bool IsBusy;
};

