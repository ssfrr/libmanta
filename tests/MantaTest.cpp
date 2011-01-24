#include "../core/Manta.h"
#include <iostream>
#include <cstring>

using namespace std;

class MantaTester: public Manta
{
   private:
      virtual void PadEvent(int id, int value)
      {
         cout << "Pad " << id << ": " << value << endl;
      }
      virtual void SliderEvent(int id, int value)
      {
         cout << "Slider " << id << ": " << value << endl;
      }
      virtual void ButtonEvent(int id, int value)
      {
         cout << "Button " << id << ": " << value << endl;
      }
};

static void waitForTransmitComplete(MantaUSB &manta)
{
   while(manta.IsTransmitting())
   {
      manta.HandleEvents();
   }
}
int main()
{
   MantaTester manta;
   manta.Connect();
   manta.SetLEDControl(Manta::PadAndButton, true);
   waitForTransmitComplete(manta);
   manta.SetLEDControl(Manta::Slider, true);
   waitForTransmitComplete(manta);

   uint8_t zeros[6];
   uint8_t effs[6];
   for(int i = 0; i < 6; ++i)
   {
      zeros[i] = 0;
      effs[i] = 0xff;
   }
   manta.SetLEDFrame(Manta::Amber, effs);
   waitForTransmitComplete(manta);
   usleep(200000);
   manta.SetLEDFrame(Manta::Red, effs);
   waitForTransmitComplete(manta);
   usleep(200000);
   manta.SetLEDFrame(Manta::Amber, effs);
   waitForTransmitComplete(manta);
   usleep(200000);
   manta.SetLEDFrame(Manta::Amber, zeros);
   waitForTransmitComplete(manta);
   usleep(200000);
   for(int i = 0; i < 6; ++i)
   {
      manta.SetLEDRow(Manta::Amber, i, 0xFF);
      waitForTransmitComplete(manta);
      usleep(200000);
      manta.SetLEDRow(Manta::Red, i, 0xFF);
      waitForTransmitComplete(manta);
      usleep(200000);
      manta.SetLEDRow(Manta::Amber, i, 0xFF);
      waitForTransmitComplete(manta);
      usleep(200000);
      manta.SetLEDRow(Manta::Amber, i, 0x00);
      waitForTransmitComplete(manta);
   }
   for(int i = 0; i < 8; ++i)
   {
      manta.SetLEDColumn(Manta::Amber, i, 0xFF);
      waitForTransmitComplete(manta);
      usleep(200000);
      manta.SetLEDColumn(Manta::Red, i, 0xFF);
      waitForTransmitComplete(manta);
      usleep(200000);
      manta.SetLEDColumn(Manta::Amber, i, 0xFF);
      waitForTransmitComplete(manta);
      usleep(200000);
      manta.SetLEDColumn(Manta::Amber, i, 0x00);
      waitForTransmitComplete(manta);
   }
   for(int i = 0; i < 4; ++i)
   {
      manta.SetButtonLED(Manta::Amber, i, true);
      waitForTransmitComplete(manta);
      usleep(200000);
      manta.SetButtonLED(Manta::Red, i, true);
      waitForTransmitComplete(manta);
      usleep(200000);
      manta.SetButtonLED(Manta::Amber, i, true);
      waitForTransmitComplete(manta);
      usleep(200000);
      manta.SetButtonLED(Manta::Amber, i, false);
      waitForTransmitComplete(manta);
   }

   for(int i = 0x80, j = 0x01; i; i >>= 1, j <<= 1)
   {
      manta.SetSliderLEDs(0, i);
      waitForTransmitComplete(manta);
      manta.SetSliderLEDs(1, j);
      waitForTransmitComplete(manta);
      usleep(100000);
   }
   manta.SetSliderLEDs(0, 0);
   waitForTransmitComplete(manta);
   manta.SetSliderLEDs(1, 0);
   waitForTransmitComplete(manta);

   for(int i = 0; i < 8; ++i)
   {
      manta.SetLEDColumn(Manta::Amber, i, 0xAA);
      waitForTransmitComplete(manta);
      usleep(50000);
      manta.SetLEDColumn(Manta::Amber, i, 0x55);
      waitForTransmitComplete(manta);
      usleep(50000);
      manta.SetLEDColumn(Manta::Amber, i, 0x00);
      waitForTransmitComplete(manta);
   }
   for(int i = 0; i < 8; ++i)
   {
      manta.SetLEDColumn(Manta::Red, i, 0xAA);
      waitForTransmitComplete(manta);
      usleep(50000);
      manta.SetLEDColumn(Manta::Red, i, 0x55);
      waitForTransmitComplete(manta);
      usleep(50000);
      manta.SetLEDColumn(Manta::Red, i, 0x00);
      waitForTransmitComplete(manta);
   }
   for(int i = 0; i < 0x40; ++i)
   {
      for(int j = 0; j < 8; ++j)
         manta.SetLEDColumn(Manta::Red, j, i);
      waitForTransmitComplete(manta);
      usleep(50000);
   }
   for(int j = 0; j < 8; ++j)
      manta.SetLEDColumn(Manta::Red, j, 0x00);
   waitForTransmitComplete(manta);
   for(int i = 0; i < 6; ++i)
   {
      manta.SetLEDRow(Manta::Amber, i, 0xAA);
      waitForTransmitComplete(manta);
      usleep(50000);
      manta.SetLEDRow(Manta::Amber, i, 0x55);
      waitForTransmitComplete(manta);
      usleep(50000);
      manta.SetLEDRow(Manta::Amber, i, 0x00);
      waitForTransmitComplete(manta);
   }
   for(int i = 0; i < 6; ++i)
   {
      manta.SetLEDRow(Manta::Red, i, 0xAA);
      waitForTransmitComplete(manta);
      usleep(50000);
      manta.SetLEDRow(Manta::Red, i, 0x55);
      waitForTransmitComplete(manta);
      usleep(50000);
      manta.SetLEDRow(Manta::Red, i, 0x00);
      waitForTransmitComplete(manta);
   }
   for(int i = 0; i < 256; ++i)
   {
      for(int j = 0; j < 6; ++j)
         manta.SetLEDRow(Manta::Red, j, i);
      waitForTransmitComplete(manta);
      usleep(50000);
   }
   for(int j = 0; j < 6; ++j)
      manta.SetLEDRow(Manta::Red, j, 0x00);
   waitForTransmitComplete(manta);

   manta.SetLEDControl(Manta::PadAndButton, false);
   waitForTransmitComplete(manta);
   manta.SetLEDControl(Manta::Slider, false);
   waitForTransmitComplete(manta);

   while(1)
   {
      manta.HandleEvents();
   }

   return 0;
}
