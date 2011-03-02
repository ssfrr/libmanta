#ifndef _COREMIDIMANAGER_H
#define _COREMIDIMANAGER_H

#include "MIDImanager.h"
#include <CoreFoundation/CoreFoundation.h>
#include <CoreMIDI/MIDIServices.h>
#include <CoreAudio/HostTime.h>

void midimsg_die(char *errmsg);
void midimsg_attempt(OSStatus result, char* errmsg);

class CoreMidiManager : public MidiManager
{
public:
	CoreMidiManager();
	~CoreMidiManager();
private:
	void InitializeMIDI();
	void SendMIDI(char actionType, int noteNum, int value);
	
	MIDIClientRef m_midiclient;
	MIDIEndpointRef m_midiendpoint;
};

#endif