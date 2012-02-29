#include "settings/MantaMidiSettings.h"
#include "midimanager/RtMidiManager.h"
#include <cstring>
#include <stdio.h>
#include <iostream>

using namespace std;

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
  RtMidiManager manta = RtMidiManager(&options);

  bool bRun = true;
  
  while (bRun)
  {
      try
      {
          manta.Connect();
          while (bRun)
          {
              manta.HandleEvents();
              usleep(5000);
          }
      }
      catch (MantaNotFoundException e)
      {
          cout << "Couldn't find a manta...retrying" << endl;
      }
      catch (MantaCommunicationException e)
      {
          cout << "Connection with Manta Interrupted...retrying" << endl;
      }
      if(bRun)
      {
          // This is a retry, not a quit, so wait a bit before retrying
          sleep(1);
      }
  }
  
  return 0;
}

void ErrorHandler(int num, const char *msg, const char *path)
{
   std::cout << "mantamidi server error " << num << " in path " << path << ": " << msg << std::endl;
}
