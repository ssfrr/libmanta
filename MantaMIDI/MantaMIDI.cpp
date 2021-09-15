/* MantaMIDI.cpp
 *
 * This is the main file for the command-line version of hte MantaMIDI software.
 *
 * MantaMIDI reads raw Manta data from USB, and sends it out as MIDI messages.
 * We use the RTMIDI package for handling MIDI messages.
 *
 * MantaMIDI is written by Christopher Jacoby.
 */

#include "settings/MantaMidiSettings.h"
#include "midimanager/RtMidiManager.h"
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

static const char *strHeading = "Snyderphonics MantaMIDI\n\tThis software translates Manta USB/HID messages to MIDI.\n"
                                "\tRun with \"-h\" for command line instructions. \n\n";

using namespace std;

void ErrorHandler(int num, const char *m, const char *path);

int main(int argc, char* argv[])
{
    cout << strHeading;                     // Print the Main Heading

    MantaMidiSettings options;              // This class stores all of the present settings
    if (options.LoadArgs(argc-1, argv+1))   // If there are command line arguments to modify the settings,
                                            // read them in. they take precedence.
    {
      RtMidiManager manta = RtMidiManager(&options);    // This handles manta interaction.
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
    else
    {
      options.PrintUsage();
      return -1;
    }
}

void ErrorHandler(int num, const char *msg, const char *path)
{
   std::cout << "mantamidi server error " << num << " in path " << path << ": " << msg << std::endl;
}
