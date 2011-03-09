#ifndef _MIDIMANAGER_H
#define _MIDIMANAGER_H

#include "../core/Manta.h"
#include "../core/MantaExceptions.h"
#include <stdint.h>

#define MANTA_PADS      48
#define MAX_MIDI_NOTES	128		// 48 for the actual manta array...
static int midi_channel = 3;

typedef struct
{
  int timeOn;
  int value;
  int lastValue;
} MidiNote;

class MidiManager : public Manta
{
 public:
  MidiManager();
  ~MidiManager();
  
 protected:
  virtual void InitializeMIDI() = 0;
  virtual void SendMIDI(char actionType, int noteNum, int value) = 0;
  
 private:
  /* Manta events */
  virtual void PadEvent(int id, int value);
  virtual void SliderEvent(int id, int value);
  virtual void ButtonEvent(int id, int value);
  virtual void PadVelocityEvent(int id, int value);
  virtual void ButtonVelocityEvent(int id, int value);

  void InitializePadMidiValues();

  /* MIDI handling */
  void Send_Volume(int value);
  void Send_Aftertouch(int noteNum, int value);
  void Send_ChannelPressure(int value);
  void Send_NoteOn(int noteNum, int value);
  void Send_NoteOff(int noteNum, int value);
  
  void ComposeMIDIMessage(char actionType, int noteNum, int value, uint8_t *msg);
  
  int m_noteToKeyMap[MANTA_PADS];
  MidiNote m_notes[MAX_MIDI_NOTES];
};

#endif
