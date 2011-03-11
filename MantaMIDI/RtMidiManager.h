#ifndef _RTMIDIMANAGER_H
#define _RTMIDIMANAGER_H

#include "MIDImanager.h"
#include "rtmidi/RtMidi.h"

class OptionHolder;

class RtMidiManager : public MidiManager
{
public:
	RtMidiManager(OptionHolder &options);
	~RtMidiManager();
private:
	void InitializeMIDI();
	void SendMIDI(unsigned char ucChannel, char actionType, int noteNum, int value);

	bool ChooseMidiPort();

	//RtMidiIn *m_midiIn;
	RtMidiOut *m_midiOut;
};

#endif
