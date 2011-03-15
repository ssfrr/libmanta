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

void RtMidiManager::SendMIDI(unsigned char data[], int nBytes)
{
  // Send out a series of MIDI messages
  try
    {
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
