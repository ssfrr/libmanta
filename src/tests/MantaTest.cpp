#include "Manta.h"
#include <iostream>
#include <cstring>

using namespace std;

void PadEvent(int id, int value)
{
   cout << "Pad " << id << ": " << value << endl;
}

void SliderEvent(int id, int value)
{
   cout << "Slider " << id << ": " << value << endl;
}

void ButtonEvent(int id, int value)
{
   cout << "Button " << id << ": " << value << endl;
}

int main()
{
   Manta manta;
   manta.SetPadCallback(PadEvent);
   manta.SetSliderCallback(SliderEvent);
   manta.SetButtonCallback(ButtonEvent);
   manta.SetLEDControl(Manta::PadAndButton, true);
   manta.SetLEDControl(Manta::Slider, true);

   uint8_t zeros[6];
   uint8_t effs[6];
   for(int i = 0; i < 6; ++i)
   {
      zeros[i] = 0;
      effs[i] = 0xff;
   }
   manta.SetLEDFrame(Manta::Amber, effs);
   usleep(400000);
   manta.SetLEDFrame(Manta::Amber, zeros);
   usleep(400000);
   manta.SetLEDFrame(Manta::Red, effs);
   usleep(400000);
   manta.SetLEDFrame(Manta::Red, zeros);
   usleep(400000);
   //manta.SetLEDFrame(Manta::Amber, effs);
   //manta.SetLEDFrame(Manta::Red, effs);
   //usleep(400000);
   //manta.SetLEDFrame(Manta::Amber, zeros);
   //manta.SetLEDFrame(Manta::Red, zeros);
   //usleep(400000);
   for(int i = 0; i < 8; ++i)
   {
      manta.SetLEDColumn(Manta::Amber, i, 0xFF);
      manta.SetLEDColumn(Manta::Red, i, 0xFF);
      usleep(400000);
      manta.SetLEDColumn(Manta::Amber, i, 0x00);
      manta.SetLEDColumn(Manta::Red, i, 0x00);
   }
   for(int i = 0; i < 4; ++i)
   {
      manta.SetButtonLEDs(Manta::Amber, i, true);
      usleep(200000);
      manta.SetButtonLEDs(Manta::Amber, i, false);
   }

   for(int i = 0; i < 4; ++i)
   {
      manta.SetButtonLEDs(Manta::Red, i, true);
      usleep(200000);
      manta.SetButtonLEDs(Manta::Red, i, false);
   }

   for(int i = 0x80, j = 0x01; i; i >>= 1, j <<= 1)
   {
      manta.SetSliderLEDs(0, i);
      manta.SetSliderLEDs(1, j);
      usleep(100000);
   }
   manta.SetSliderLEDs(0, 0);
   manta.SetSliderLEDs(1, 0);

   for(int i = 0; i < 8; ++i)
   {
      manta.SetLEDColumn(Manta::Amber, i, 0xAA);
      usleep(50000);
      manta.SetLEDColumn(Manta::Amber, i, 0x55);
      usleep(50000);
      manta.SetLEDColumn(Manta::Amber, i, 0x00);
   }
   for(int i = 0; i < 8; ++i)
   {
      manta.SetLEDColumn(Manta::Red, i, 0xAA);
      usleep(50000);
      manta.SetLEDColumn(Manta::Red, i, 0x55);
      usleep(50000);
      manta.SetLEDColumn(Manta::Red, i, 0x00);
   }
   for(int i = 0; i < 0x40; ++i)
   {
      for(int j = 0; j < 8; ++j)
         manta.SetLEDColumn(Manta::Red, j, i);
      usleep(50000);
   }
   for(int j = 0; j < 8; ++j)
      manta.SetLEDColumn(Manta::Red, j, 0x00);
   for(int i = 0; i < 6; ++i)
   {
      manta.SetLEDRow(Manta::Amber, i, 0xAA);
      usleep(50000);
      manta.SetLEDRow(Manta::Amber, i, 0x55);
      usleep(50000);
      manta.SetLEDRow(Manta::Amber, i, 0x00);
   }
   for(int i = 0; i < 6; ++i)
   {
      manta.SetLEDRow(Manta::Red, i, 0xAA);
      usleep(50000);
      manta.SetLEDRow(Manta::Red, i, 0x55);
      usleep(50000);
      manta.SetLEDRow(Manta::Red, i, 0x00);
   }
   for(int i = 0; i < 256; ++i)
   {
      for(int j = 0; j < 6; ++j)
         manta.SetLEDRow(Manta::Red, j, i);
      usleep(50000);
   }
   for(int j = 0; j < 6; ++j)
      manta.SetLEDRow(Manta::Red, j, 0x00);

   manta.SetLEDControl(Manta::PadAndButton, false);
   manta.SetLEDControl(Manta::Slider, false);

   manta.StartPoll();
   return 0;
}
