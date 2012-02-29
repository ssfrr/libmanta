#include "settings/MantaMidiSettings.h"
#include "midimanager/RtMidiManager.h"
#include <cstring>
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
  MantaMidiSettings options(argc-1, argv+1);
  RtMidiManager *manta = new RtMidiManager(&options);
  
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
      manta->Initialize();
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
