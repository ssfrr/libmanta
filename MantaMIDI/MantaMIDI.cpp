//#include "MIDImanager.h"
#include "RtMidiManager.h"
#include <cstring>
//#include <unistd.h>
#include <stdio.h>
#include <iostream>

bool bDebugMode = false;

void ErrorHandler(int num, const char *m, const char *path);

int main(int argc, char** argv)
{
  if (argc > 1 && argv[1] && argv[1][0] == '1')
    {
      bDebugMode = true;
      std::cout << "Debug Mode: ON\n";
    }
  
  MidiManager *manta = new RtMidiManager();
  
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
