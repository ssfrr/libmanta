#include "../core/Manta.h"
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdarg>

using namespace std;

class MantaTester: public Manta
{
   private:
      virtual void PadEvent(int row, int column, int id, int value)
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
      virtual void PadVelocityEvent(int row, int column, int id, int value)
      {
         cout << "Pad Velocity " << id << ": " << value << endl;
      }
      virtual void ButtonVelocityEvent(int id, int value)
      {
         cout << "Button Velocity " << id << ": " << value << endl;
      }
      virtual void DebugPrint(const char *fmt, ...)
      {
         va_list args;
         va_start(args, fmt);
         vprintf(fmt, args);
         printf("\n");
         va_end (args);
      }
};

int main()
{
   MantaTester manta;
   manta.Connect();
   manta.SetLEDControl(MantaServer::PadAndButton, true);
   Manta::HandleEvents();
   manta.SetLEDControl(MantaServer::Slider, true);
   Manta::HandleEvents();

   uint8_t effs[6];
   for(int i = 0; i < 6; ++i)
   {
      effs[i] = 0xff;
   }
   manta.SetPadLEDFrame(MantaServer::Amber, effs);
   Manta::HandleEvents();
   usleep(200000);
   manta.SetPadLEDFrame(MantaServer::Red, effs);
   Manta::HandleEvents();
   usleep(200000);
   manta.SetPadLEDFrame(MantaServer::Amber, effs);
   Manta::HandleEvents();
   usleep(200000);
   manta.SetPadLEDFrame(MantaServer::Off, effs);
   Manta::HandleEvents();
   usleep(200000);
   for(int i = 0; i < 6; ++i)
   {
      manta.SetPadLEDRow(MantaServer::Amber, i, 0xFF);
      Manta::HandleEvents();
      usleep(200000);
      manta.SetPadLEDRow(MantaServer::Red, i, 0xFF);
      Manta::HandleEvents();
      usleep(200000);
      manta.SetPadLEDRow(MantaServer::Amber, i, 0xFF);
      Manta::HandleEvents();
      usleep(200000);
      manta.SetPadLEDRow(MantaServer::Off, i, 0xFF);
      Manta::HandleEvents();
   }
   for(int i = 0; i < 8; ++i)
   {
      manta.SetPadLEDColumn(MantaServer::Amber, i, 0xFF);
      Manta::HandleEvents();
      usleep(200000);
      manta.SetPadLEDColumn(MantaServer::Red, i, 0xFF);
      Manta::HandleEvents();
      usleep(200000);
      manta.SetPadLEDColumn(MantaServer::Amber, i, 0xFF);
      Manta::HandleEvents();
      usleep(200000);
      manta.SetPadLEDColumn(MantaServer::Off, i, 0xFF);
      Manta::HandleEvents();
   }
   manta.SetPadLED(MantaServer::Red, 0);
   Manta::HandleEvents();
   usleep(100000);
   for(int i = 1; i < 48; ++i)
   {
      manta.SetPadLED(MantaServer::Red, i);
      manta.SetPadLED(MantaServer::Amber, i - 1);
      Manta::HandleEvents();
      usleep(100000);
   }
   manta.SetPadLEDFrame(MantaServer::Off, effs);
   for(int i = 0; i < 4; ++i)
   {
      manta.SetButtonLED(MantaServer::Amber, i);
      Manta::HandleEvents();
      usleep(200000);
      manta.SetButtonLED(MantaServer::Red, i);
      Manta::HandleEvents();
      usleep(200000);
      manta.SetButtonLED(MantaServer::Amber, i);
      Manta::HandleEvents();
      usleep(200000);
      manta.SetButtonLED(MantaServer::Off, i);
      Manta::HandleEvents();
   }

   for(int i = 0x80, j = 0x01; i; i >>= 1, j <<= 1)
   {
      manta.SetSliderLED(MantaServer::Off, 0, 0xFF);
      manta.SetSliderLED(MantaServer::Off, 1, 0xFF);
      manta.SetSliderLED(MantaServer::Amber, 0, i);
      manta.SetSliderLED(MantaServer::Amber, 1, j);
      Manta::HandleEvents();
      usleep(100000);
   }
   manta.SetSliderLED(MantaServer::Off, 0, 0xFF);
   manta.SetSliderLED(MantaServer::Off, 1, 0xFF);
   Manta::HandleEvents();


   manta.SetLEDControl(MantaServer::PadAndButton, false);
   Manta::HandleEvents();
   manta.SetLEDControl(MantaServer::Slider, false);
   Manta::HandleEvents();

   while(1)
   {
      Manta::HandleEvents();
      usleep(1000);
   }

   return 0;
}
