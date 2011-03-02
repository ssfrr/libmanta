#include "MIDImanager.h"
//#include "coremidimanager.h"
#include "../core/Manta.h"
#include "../core/MantaExceptions.h"
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <stdexcept>

class MantaMIDI : public Manta
{
public:
	MantaMIDI();
	~MantaMIDI();
private:
	virtual void PadEvent(int id, int value);
	virtual void SliderEvent(int id, int value);
	virtual void ButtonEvent(int id, int value);
	virtual void PadVelocityEvent(int id, int value);
	virtual void ButtonVelocityEvent(int id, int value);
	
	MidiManager *m_midiManager;
};

void ErrorHandler(int num, const char *m, const char *path);

MantaMIDI::LEDState getLEDStateFromString(const char *stateString);

MantaMIDI::MantaMIDI()
{
	//m_midiManager = new CoreMidiManager();
	m_midiManager = new MidiManager();
}

MantaMIDI::~MantaMIDI()
{
	delete m_midiManager;
}

void MantaMIDI::PadEvent(int id, int value)
{
	std::cout << "PadEvent: " << id << ", " << value << "\n";
  //lo_send(OSCAddress, "/Manta/Continuous/Pad", "ii", id, value);
}

void MantaMIDI::SliderEvent(int id, int value)
{
	std::cout << "SliderEvent: " << id << ", " << value << "\n";
  //lo_send(OSCAddress, "/Manta/Continuous/Slider", "ii", id, value);
}

void MantaMIDI::ButtonEvent(int id, int value)
{
	std::cout << "ButtonEvent: " << id << ", " << value << "\n";
  //lo_send(OSCAddress, "/Manta/Continuous/Button", "ii", id, value);
}

void MantaMIDI::PadVelocityEvent(int id, int value)
{
	std::cout << "PadVelocityEvent: " << id << ", " << value << "\n";
  //lo_send(OSCAddress, "/Manta/Velocity/Pad", "ii", id, value);
}

void MantaMIDI::ButtonVelocityEvent(int id, int value)
{
	std::cout << "ButtonVelocityEvent: " << id << ", " << value << "\n";
  //lo_send(OSCAddress, "/Manta/Velocity/Button", "ii", id, value);
}

int main(void)
{
   MantaMIDI manta;
   do
   {
      try
      {
         manta.Connect();
      }
      catch(MantaNotFoundException &e)
      {
         std::cout << "Could not find an attached Manta. Retrying..." << std::endl;
         sleep(1);
      }
   } while(! manta.IsConnected());
   std::cout << "Manta Connected" << std::endl;
   try
   {
      while(1)
      {
         manta.HandleEvents();
      }
   }
   catch(MantaCommunicationException &e)
   {
      std::cout << "Communication with Manta interrupted, exiting..." << std::endl;
   }
   return 0;
}

void ErrorHandler(int num, const char *msg, const char *path)
{
   std::cout << "liblo server error " << num << " in path " << path << 
      ": " << msg << std::endl;
}

MantaMIDI::LEDState getLEDStateFromString(const char *stateString)
{
   if(strcmp(stateString, "Red") == 0 ||
         strcmp(stateString, "red") == 0)
   {
      return MantaMIDI::Red;
   }
   else if(strcmp(stateString, "Amber") == 0 ||
         strcmp(stateString, "amber") == 0)
   {
      return MantaMIDI::Amber;
   }
   else if(strcmp(stateString, "Off") == 0 ||
         strcmp(stateString, "off") == 0)
   {
      return MantaMIDI::Off;
   }
   else
   {
      throw std::invalid_argument("Invalid LEDState string");
   }
}
