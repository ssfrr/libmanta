#include "rtmidimanager.h"
#include <stdio.h>
#include <cstdlib>
#include <iostream>

extern bool bDebugMode;

RtMidiManager::RtMidiManager() /*:
				 m_midiOut(0)*/
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

void RtMidiManager::SendMIDI(char actionType, int noteNum, int value)
{
  std::vector<unsigned char> message;
  int baseNote = 30;

  // Send out a series of MIDI messages
  try
    {
      if(actionType == 'O')
	{
	  // Note On: 144, 64, 90
	  if (bDebugMode)
	    printf("Note on: %d\n", baseNote + noteNum);
	  message.push_back(144);
	  message.push_back(baseNote + noteNum);
	  message.push_back(100);
	  m_midiOut->sendMessage( &message );
	}
      else if (actionType == 'o')
	{
	  // Note Off: 128, 64, 40
	  if (bDebugMode)
	    printf("Note off: %d\n", baseNote + noteNum);
	  message.push_back(128);
	  message.push_back(30 + noteNum);
	  message.push_back(100);
	  m_midiOut->sendMessage( &message );
	}
      /* else if (actionType == 'A')
	{
	  if (bDebugMode)
	    printf("aftertourch: %d", value);
	  message.push_back(0xA0);
	  message.push_back(baseNote + noteNum);
	  message.push_back(value);
	  m_midiOut->sendMessage( &message );
	  }*/

      /*
      // Program change: 192, 5
      message.push_back( 192 );
      message.push_back( 5 );
      //m_midiOut->sendMessage( &message );
	  
      // Control Change: 176, 7, 100 (volume)
      message[0] = 176;
      message[1] = 7;
      message.push_back( 100 );
      //m_midiOut->sendMessage( &message );
      */      
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
