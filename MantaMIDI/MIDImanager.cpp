#include "OptionHolder.h"
#include "MIDImanager.h"
#include <cstring>
#include <stdio.h>
#include <iostream>

MidiManager::MidiManager(OptionHolder &options) :
  m_options(options)
{
  InitializeMapValues();
}

MidiManager::~MidiManager()
{
	
}

void MidiManager::PadEvent(int id, int value)
{
  if (m_options.GetDebugMode())
    std::cout << "PadEvent: " << id << ", " << value << "\n";

  if (!m_options.GetUseVelocity())
    SendPadMIDI(id, value);
}

void MidiManager::SliderEvent(int id, int value)
{
  if (m_options.GetDebugMode())
    std::cout << "SliderEvent: " << id << ", " << value << "\n";

  if (!m_options.GetUseVelocity())
    SendSliderMIDI(id, value);
}

void MidiManager::ButtonEvent(int id, int value)
{
  if (m_options.GetDebugMode())
    std::cout << "ButtonEvent: " << id << ", " << value << "\n";

  SendButtonMIDI(id, value);
}

void MidiManager::PadVelocityEvent(int id, int value)
{
  if (m_options.GetDebugMode())
    std::cout << "PadVelocityEvent: " << id << ", " << value << "\n";

  if (m_options.GetUseVelocity())
    SendPadMIDI(id, value);
}

void MidiManager::ButtonVelocityEvent(int id, int value)
{
  if (m_options.GetDebugMode())
    std::cout << "ButtonVelocityEvent: " << id << ", " << value << "\n";

  if (m_options.GetUseVelocity())
    SendPadMIDI(id, value);
}

void MidiManager::InitializeMapValues()
{
  for(int i = 0; i < MANTA_PADS; ++i)
      m_padToNoteMap[i] = m_options.GetBasePadMidi() + i;
}

void MidiManager::SendPadMIDI(int noteNum, int value)
{
  int channel = m_options.GetPadEventChannel();
  int midiNote = m_padToNoteMap[noteNum];
  MidiNote &note = m_padNotes[midiNote];

  if (m_options.GetUseVelocity())
    Send_NoteOn(channel, midiNote, value);
  else
    {
      if (0 == note.lastValue && value > 0)
	Send_NoteOn(channel, midiNote, 100);
      else if (value == 0)
	Send_NoteOff(channel, midiNote, 0);
    }
  //else
  //Send_Aftertouch(midiNote, value);

  note.lastValue = note.value;
  note.value = value;
}

void MidiManager::SendSliderMIDI(int noteNum, int value)
{
  int channel = m_options.GetSliderEventChannel();
}

void MidiManager::SendButtonMIDI(int noteNum, int value)
{
  int channel = m_options.GetButtonEventChannel();
}

void MidiManager::Send_Volume(int channel, int value)
{
  SendMIDI( channel, 'V', 7 /* course volume */, value );
}

void MidiManager::Send_Aftertouch(int channel, int noteNum, int value)
{
  SendMIDI( channel, 'A', noteNum, value );
}

void MidiManager::Send_ChannelPressure(int channel, int value)
{
  SendMIDI( channel, 'C', value, -1 );
}

void MidiManager::Send_NoteOn(int channel, int noteNum, int velocity)
{
  SendMIDI( channel, 'O', noteNum, velocity );
}

void MidiManager::Send_NoteOff(int channel, int noteNum, int velocity)
{
  SendMIDI( channel, 'o', noteNum, velocity );
}
