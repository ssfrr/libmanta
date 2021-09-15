#include "../settings/MantaMidiSettings.h"
#include "RtMidiManager.h"
#include <stdio.h>
#include <cstdlib>
#include <iostream>

void MidiReadThread( double deltatime, std::vector< unsigned char > *message, void *userData )
{
 /* unsigned int nBytes = message->size();
  for ( unsigned int i=0; i<nBytes; i++ )
    std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
  if ( nBytes > 0 )
  std::cout << "stamp = " << deltatime << std::endl;*/

  RtMidiManager *manta = (RtMidiManager *)userData;

  int first = message->at(0);
  int second = message->at(1);
  int third = message->at(2);

  int padIndex = -1;
  int velocity = 0;

  Manta::LEDState resultColor = Manta::Off;

  if (first == 144)
  {
      if (second >= 1 && second <= 48)
      {
          padIndex = second - 1;
          velocity = third;

          if(velocity > 64)
              resultColor = Manta::Red;
          else if (velocity > 0)
              resultColor = Manta::Amber;
          else
              resultColor = Manta::Off;

      }
  }
  else if (first == 128)
    {
        if (second >= 1 && second <= 48)
        {
            padIndex = second - 1;
            resultColor = Manta::Off;
        }
    }

  //int index = manta->GetOptions()->GetPadFromMidi(second, resultColor);

  /*if ( 0 == third )
    resultColor = Manta::Off;*/

  if (padIndex >= 0)
  {
        manta->SetPadLED(resultColor, padIndex);
  }
}

RtMidiManager::RtMidiManager(MantaMidiSettings *options) :
  MidiManager(options)
{
  InitializeMIDI();
}

RtMidiManager::~RtMidiManager()
{
  delete m_midiIn;
  delete m_midiOut;
}

void RtMidiManager::InitializeMIDI()
{
  // RtMidiIn constructor
  try {
    m_midiIn = new RtMidiIn();
    m_midiOut = new RtMidiOut();

    ChooseMidiPort();

    m_midiIn->setCallback( &MidiReadThread, this );
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
#ifdef _WIN32 // also includes 64-bit windows
  std::string keyHit;
  // Check ports for output
  std::string portName;
  unsigned int i = 0, nOutPorts = m_midiOut->getPortCount();
  if ( nOutPorts == 0 ) {
    std::cout << "No output ports available!" << std::endl;
    return false;
  }

  unsigned int j = 0, nInPorts = m_midiIn->getPortCount();
  if ( nInPorts == 0 ){
    std::cout << "No input ports available!" << std::endl;
    return false;
  }

  if ( nOutPorts == 1 ) {
    std::cout << "\nOpening " << m_midiOut->getPortName() << " for output" << std::endl;
  }
  else {
    for ( i=0; i<nOutPorts; i++ ) {
      portName = m_midiOut->getPortName(i);
      std::cout << "  Output port #" << i << ": " << portName << '\n';
    }

    do {
      std::cout << "\nChoose a port number: ";
      std::cin >> i;
    } while ( i >= nOutPorts );
  }

  std::cout << "\n";
  m_midiOut->openPort( i );

  if ( nInPorts == 1 ) {
    std::cout << "\nOpening " << m_midiIn->getPortName() << " for input" << std::endl;
  }
  else {
    for ( i=0; i<nInPorts; i++ ) {
      portName = m_midiIn->getPortName(i);
      std::cout << "  Input port #" << i << ": " << portName << '\n';
    }

    do {
      std::cout << "\nChoose a port number: ";
      std::cin >> i;
    } while ( i >= nInPorts );
  }

  std::getline( std::cin, keyHit ); // used to clear out stdin
  m_midiIn->openPort( i );

#else // not _WIN32

  m_midiIn->openVirtualPort("Manta In");
  m_midiOut->openVirtualPort("Manta Out");

#endif

  return true;
}
