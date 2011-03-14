//#include "MIDImanager.h"
#include "OptionHolder.h"
#include "RtMidiManager.h"
#include <cstring>
//#include <unistd.h>
#include <stdio.h>
#include <iostream>

void ErrorHandler(int num, const char *m, const char *path);

void usage()
{
  printf("Usage: MantaMIDI [options]\n");
  printf("\n");
  printf("Options:\n");
  printf("\n");
}

int main(int argc, char* argv[])
{
  OptionHolder options(argc-1, argv+1);
  MidiManager *manta = new RtMidiManager(options);
  
  do
    {
      try
	{
	  manta->Connect();
	}
      catch(MantaNotFoundException &e)
	{
	  std::cout << "Could not find an attached Manta. Retrying..." << std::endl;
	  sleep(1);
	}
    } while(! manta->IsConnected());

  std::cout << "Manta Connected" << std::endl;
  try
    {
      manta->SetLEDControl(Manta::PadAndButton, true);
      waitForTransmitComplete(*manta);
      manta->SetLEDControl(Manta::Slider, true);
      waitForTransmitComplete(*manta);

      uint8_t effs[6];
      for(int i = 0; i < 6; ++i)
	{
	  effs[i] = 0xff;
	}
      manta->SetPadLEDFrame(Manta::Off, effs);
      waitForTransmitComplete(*manta);

      /*manta->SetLEDControl(Manta::PadAndButton, false);
      waitForTransmitComplete(*manta);
      manta->SetLEDControl(Manta::Slider, false);
      waitForTransmitComplete(*manta);*/

      while(1)
	{
	  manta->HandleEvents();
	}
    }
  catch(MantaCommunicationException &e)
    {
      std::cout << "Communication with Manta interrupted, exiting..." << std::endl;
    }
  
  delete manta;
  
  return 0;
}

void ErrorHandler(int num, const char *msg, const char *path)
{
   std::cout << "mantamidi server error " << num << " in path " << path << ": " << msg << std::endl;
}
