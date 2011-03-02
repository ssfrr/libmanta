#include "MIDImanager.h"

MidiManager::MidiManager()
{
	InitializeMIDI();
}

MidiManager::~MidiManager()
{
	
}

void MidiManager::Send_Volume(int value)
{
}

void MidiManager::Send_Aftertouch(int noteNum, int value)
{
}

void MidiManager::Send_ChannelPressure(int value)
{
}

void MidiManager::Send_NoteOn(int noteNum, int value)
{
}

void MidiManager::Send_NoteOff(int noteNum, int value)
{
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
	/* aftertouch */
	else if (actionType == 'A') 
	{ 
		msg[0] = 0xA0;
		msg[2] = 82;
	}
	/* channel pressure */
	else if (actionType == 'C') 
	{ 
		msg[0] = 0xD0;
		msg[2] = 0;
	}    
	/* volume */
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
}