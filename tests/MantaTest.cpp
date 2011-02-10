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
      virtual void PadVelocityEvent(int id, int value)
      {
         cout << "Pad Velocity " << id << ": " << value << endl;
      }
      virtual void ButtonVelocityEvent(int id, int value)
      {
         cout << "Button Velocity " << id << ": " << value << endl;
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

   uint8_t effs[6];
   for(int i = 0; i < 6; ++i)
   {
      effs[i] = 0xff;
   }
   manta.SetPadLEDFrame(Manta::Amber, effs);
   waitForTransmitComplete(manta);
   usleep(200000);
   manta.SetPadLEDFrame(Manta::Red, effs);
   waitForTransmitComplete(manta);
   usleep(200000);
   manta.SetPadLEDFrame(Manta::Amber, effs);
   waitForTransmitComplete(manta);
   usleep(200000);
   manta.SetPadLEDFrame(Manta::Off, effs);
   waitForTransmitComplete(manta);
   usleep(200000);
   for(int i = 0; i < 6; ++i)
   {
      manta.SetPadLEDRow(Manta::Amber, i, 0xFF);
      waitForTransmitComplete(manta);
      usleep(200000);
      manta.SetPadLEDRow(Manta::Red, i, 0xFF);
      waitForTransmitComplete(manta);
      usleep(200000);
      manta.SetPadLEDRow(Manta::Amber, i, 0xFF);
      waitForTransmitComplete(manta);
      usleep(200000);
      manta.SetPadLEDRow(Manta::Off, i, 0xFF);
      waitForTransmitComplete(manta);
   }
   for(int i = 0; i < 8; ++i)
   {
      manta.SetPadLEDColumn(Manta::Amber, i, 0xFF);
      waitForTransmitComplete(manta);
      usleep(200000);
      manta.SetPadLEDColumn(Manta::Red, i, 0xFF);
      waitForTransmitComplete(manta);
      usleep(200000);
      manta.SetPadLEDColumn(Manta::Amber, i, 0xFF);
      waitForTransmitComplete(manta);
      usleep(200000);
      manta.SetPadLEDColumn(Manta::Off, i, 0xFF);
      waitForTransmitComplete(manta);
   }
   manta.SetPadLED(Manta::Red, 0);
   waitForTransmitComplete(manta);
   usleep(100000);
   for(int i = 1; i < 48; ++i)
   {
      manta.SetPadLED(Manta::Red, i);
      manta.SetPadLED(Manta::Amber, i - 1);
      waitForTransmitComplete(manta);
      usleep(100000);
   }
   manta.SetPadLEDFrame(Manta::Off, effs);
   for(int i = 0; i < 4; ++i)
   {
      manta.SetButtonLED(Manta::Amber, i);
      waitForTransmitComplete(manta);
      usleep(200000);
      manta.SetButtonLED(Manta::Red, i);
      waitForTransmitComplete(manta);
      usleep(200000);
      manta.SetButtonLED(Manta::Amber, i);
      waitForTransmitComplete(manta);
      usleep(200000);
      manta.SetButtonLED(Manta::Off, i);
      waitForTransmitComplete(manta);
   }

   for(int i = 0x80, j = 0x01; i; i >>= 1, j <<= 1)
   {
      manta.SetSliderLED(Manta::Off, 0, 0xFF);
      manta.SetSliderLED(Manta::Off, 1, 0xFF);
      manta.SetSliderLED(Manta::Amber, 0, i);
      manta.SetSliderLED(Manta::Amber, 1, j);
      waitForTransmitComplete(manta);
      usleep(100000);
   }
   manta.SetSliderLED(Manta::Off, 0, 0xFF);
   manta.SetSliderLED(Manta::Off, 1, 0xFF);
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
