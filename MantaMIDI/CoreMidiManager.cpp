#include "MIDImanager.h"
#include "coremidimanager.h"

void midimsg_die(char *errmsg) 
{
	printf("%s\n",errmsg);
	exit(-1);
}

void midimsg_attempt(OSStatus result, char* errmsg) 
{
	if (result != noErr) {
		midimsg_die(errmsg);
	}
}

CoreMidiManager::CoreMidiManager()
{
	InitializeMIDI();
}

CoreMidiManager::~CoreMidiManager()
{
}

void CoreMidiManager::InitializeMIDI()
{
	/*midimsg_attempt(MIDIClientCreate
	 (CFStringCreateWithCString(NULL, "Manta", kCFStringEncodingASCII), 
	 NULL, NULL, &m_midiclient), "creating OS-X MIDI client object.");
	 
	 midimsg_attempt(MIDISourceCreate
	 (m_midiclient, 
	 CFStringCreateWithCString(NULL, "Manta", kCFStringEncodingASCII), 
	 &m_midiendpoint), "creating OS-X virtual MIDI source.");*/
}

#define PACKET_BUF_SIZE	(3+0)  // 3 for message, extra for ??
void CoreMidiManager::SendMIDI(char actionType, int noteNum, int value)
{
	/*uint8_t buffer[PACKET_BUF_SIZE];
	 uint8_t msg[3];
	 
	 MIDIPacketList *packetList = (MIDIPacketList*) buffer;
	 MidiPacket *curPacket = MIDIPacketListInit(packetList);
	 
	 curPacket = MIDIPacketListAdd(packetList,
	 PACKET_BUF_SIZE,
	 curPacket,
	 AudioGetCurrentHostTime(),
	 actionType == 'C' ? 2 : 3,
	 msg);
	 if (!curPacket)
	 midimsg_die("packet list allocation failed");
	 
	 midimsg_attempt(MIDIReceived(m_midiendpoint, packetList), "error sending midi");*/
}