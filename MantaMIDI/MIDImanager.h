#ifndef _MIDIMANAGER_H
#define _MIDIMANAGER_H

#include <stdint.h>

#define MAX_MIDI_NOTES	128		// 48 for the actual manta array...
static int midi_channel = 3;

typedef struct
{
	int timeOn;
	int value;
	int lastValue;
} MidiNote;

class MidiManager
{
public:
	MidiManager();
	~MidiManager();
	
	void Send_Volume(int value);
	void Send_Aftertouch(int noteNum, int value);
	void Send_ChannelPressure(int value);
	void Send_NoteOn(int noteNum, int value);
	void Send_NoteOff(int noteNum, int value);
	
protected:
	virtual void InitializeMIDI() = 0;
	virtual void SendMIDI(char actionType, int noteNum, int value) = 0;
	
private:
	void ComposeMIDIMessage(char actionType, int noteNum, int value, uint8_t *msg);
	 
	MidiNote m_notes[MAX_MIDI_NOTES];
};

#endif