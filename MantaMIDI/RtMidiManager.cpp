#include "OptionHolder.h"
#include "RtMidiManager.h"
#include <stdio.h>
#include <cstdlib>
#include <iostream>

RtMidiManager::RtMidiManager(OptionHolder &options) :
  MidiManager(options)
{
  InitializeMIDI();
}

RtMidiManager::~RtMidiManager()
{
  //delete m_midiIn;
  delete m_midiOut;
}

void RtMidiManager::InitializeMIDI()
{
  // RtMidiIn constructor
  try {
    // m_midiIn = new RtMidiIn();
    m_midiOut = new RtMidiOut();

    ChooseMidiPort();
  }
  catch (RtError &error) {
    // Handle the exception here
    error.printMessage();
    }
}

void RtMidiManager::SendMIDI(unsigned char ucChannel, MidiActionType actionType, int noteNum, int value)
{
  unsigned char data[3];
  int nBytes = 0;

  // Send out a series of MIDI messages
  try
    {
      // Note Off: 128, Note, Velocity
      if (actionType == atNoteOff)
	{
	  data[0] = 0x80 + ucChannel; // 128
	  data[1] = noteNum;
	  data[2] = value;
	  nBytes = 3;

	  if (m_options.GetDebugMode())
	    printf("Note off: %d %d %d\n", data[0], data[1], data[2]);
	}
      // Note On: 144/0x90, Note, Velocity
      else if(actionType == atNoteOn)
	{
	  data[0] = 0x90 + ucChannel; // 144
	  data[1] = noteNum;
	  data[2] = value;
	  nBytes = 3;
	  
	  if (m_options.GetDebugMode())
	    printf("Note on: %d %d %d\n", data[0], data[1], data[2]);
	}
      // Polyphonic Pressure (Aftertouch): 0xA0, controller # (0-119), value
      else if (actionType == atPolyphonicKeyPressure)
	{
	  data[0] = 0xA0 + ucChannel; //
	  data[1] = noteNum;
	  data[2] = value;
	  nBytes = 3;

	  if (m_options.GetDebugMode())
	    printf("Polyphonic Pressure: %d %d %d\n", data[0], data[1], data[2]);
	}
      // Control Change: 0xB0
      else if (actionType == atControlChange)
      {
	data[0] = 0xB0 + ucChannel;
	data[1] = noteNum;
	data[2] = value;
	nBytes = 3;

	if (m_options.GetDebugMode())
	  printf("Program Change: %d %d %d\n", data[0], data[1], data[2]);
      }
      // Program Change: 0xC0
      else if (actionType == atProgramChange)
	{
	  data[0] = 0xC0 + ucChannel;
	  data[1] = noteNum;
	  nBytes = 2;
	  
	  if (m_options.GetDebugMode())
	    printf("Program Change: %d %d\n", data[0], data[1]);
	}
      else if (actionType == atChannelPressure)
	{
	  if (m_options.GetDebugMode())
	    printf("Channel Pressure (aftertouch)\n");
	}
      else if (actionType == atPitchWheel)
	{
	  if (m_options.GetDebugMode())
	    printf("Pitch Wheel Change: \n");
	}
      
      std::vector<unsigned char> message;
      for (int i = 0; i < nBytes; ++i)
	message.push_back(data[i]);

      // Send the midi Message
      m_midiOut->sendMessage( &message );
    }
  catch (RtError &error) {
    error.printMessage();
  }
}

bool RtMidiManager::ChooseMidiPort()
{
  std::cout << "\nWould you like to open a virtual output port? [y/N] ";

  std::string keyHit;
  std::getline( std::cin, keyHit );
  if ( keyHit == "y" ) {
    m_midiOut->openVirtualPort( "Manta" );
    return true;
  }

  std::string portName;
  unsigned int i = 0, nPorts = m_midiOut->getPortCount();
  if ( nPorts == 0 ) {
    std::cout << "No output ports available!" << std::endl;
    return false;
  }

  if ( nPorts == 1 ) {
    std::cout << "\nOpening " << m_midiOut->getPortName() << std::endl;
  }
  else {
    for ( i=0; i<nPorts; i++ ) {
      portName = m_midiOut->getPortName(i);
      std::cout << "  Output port #" << i << ": " << portName << '\n';
    }

    do {
      std::cout << "\nChoose a port number: ";
      std::cin >> i;
    } while ( i >= nPorts );
  }

  std::cout << "\n";
  m_midiOut->openPort( i );

  return true;
}
