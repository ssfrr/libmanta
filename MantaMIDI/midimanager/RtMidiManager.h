#ifndef _RTMIDIMANAGER_H
#define _RTMIDIMANAGER_H

#include "MIDImanager.h"
#include "../rtmidi/RtMidi.h"

void MidiReadThread(double deltatime, std::vector< unsigned char > *message, void *userData);

class RtMidiManager : public MidiManager
{
public:
        RtMidiManager(MantaMidiSettings *options);
	~RtMidiManager();

private:
	void InitializeMIDI();
	void SendMIDI(unsigned char data[], int nBytes);

	bool ChooseMidiPort();

	RtMidiIn *m_midiIn;
	RtMidiOut *m_midiOut;
};

#endif
