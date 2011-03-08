#include "MIDImanager.h"
#include <cstring>
#include <stdio.h>

MidiManager::MidiManager()
{
  InitializePadMidiValues();
}

MidiManager::~MidiManager()
{
	
}

void MidiManager::HandlePadEvent(int id, int value)
{
  int midiNote = m_noteToKeyMap[id];
  MidiNote &note = m_notes[midiNote];

  if (0 == note.lastValue && value > 0)
    Send_NoteOn(midiNote, value);
  else if (0 < note.lastValue && value == 0)
    Send_NoteOff(midiNote, value);
  else
    Send_Aftertouch(midiNote, value);

  note.lastValue = note.value;
  note.value = value;
}

void MidiManager::InitializePadMidiValues()
{
  for(int i = 0; i < MANTA_PADS; ++i)
    {
      m_noteToKeyMap[i] = 48 + i;
    }
}

void MidiManager::Send_Volume(int value)
{
  SendMIDI( 'V', 7 /* course volume */, value );
}

void MidiManager::Send_Aftertouch(int noteNum, int value)
{
  SendMIDI( 'A', noteNum, value );
}

void MidiManager::Send_ChannelPressure(int value)
{
  SendMIDI( 'C', value, -1 );
}

void MidiManager::Send_NoteOn(int noteNum, int value)
{
  SendMIDI( 'O', noteNum, value );
}

void MidiManager::Send_NoteOff(int noteNum, int velocity)
{
  SendMIDI( 'o', noteNum, velocity );
}

void MidiManager::ComposeMIDIMessage(char actionType, int noteNum, int value, uint8_t *msg)
{
	// on
	if (actionType == 'O') 
	{
		msg[0] = 0x90;
		msg[2] = 82;
	}
	// off
	else if (actionType == 'o')
	{
		msg[0] = 0x80;
		msg[2] = 0;
	}
	// aftertouch 
	else if (actionType == 'A') 
	{ 
		msg[0] = 0xA0;
		msg[2] = 82;
	}
	// channel pressure 
	else if (actionType == 'C') 
	{ 
		msg[0] = 0xD0;
		msg[2] = 0;
	}    
	// volume//
	else if (actionType == 'V') 
	{ 
		msg[0] = 0xB0;
		msg[2] = 0;
	}
	
	if (value != -1) 
	{
		if (value > MAX_MIDI_NOTES)
			value = MAX_MIDI_NOTES - 1;
		msg[2] = value;
	}
	
	msg[0] += ((midi_channel - 1) & 0xFF);
	msg[1] = noteNum;

	printf("action: %c; notenum: %d, value: %d", actionType, noteNum, value);
}
