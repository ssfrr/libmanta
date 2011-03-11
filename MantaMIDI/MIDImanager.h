#ifndef _MIDIMANAGER_H
#define _MIDIMANAGER_H

#include "../core/Manta.h"
#include "../core/MantaExceptions.h"
#include <stdint.h>

#define MANTA_PADS      48
#define MANTA_SLIDERPOS 127
#define MANTA_BUTTONS   4
#define MAX_MIDI_NOTES	128		// 48 for the actual manta array...
static int midi_channel = 3;

class OptionHolder;

typedef struct
{
  int timeOn;
  int value;
  int lastValue;
} MidiNote;

class MidiManager : public Manta
{
 public:
  MidiManager(OptionHolder &options);
  ~MidiManager();
  
 protected:
  OptionHolder m_options;

  virtual void InitializeMIDI() = 0;
  virtual void SendMIDI(unsigned char ucChannel, char actionType, int noteNum, int value) = 0;
  
 private:

  /* Manta events */
  virtual void PadEvent(int id, int value);
  virtual void SliderEvent(int id, int value);
  virtual void ButtonEvent(int id, int value);
  virtual void PadVelocityEvent(int id, int value);
  virtual void ButtonVelocityEvent(int id, int value);

  void InitializeMapValues();

  /* Handling functions */
  void SendPadMIDI(int noteNum, int value);
  void SendSliderMIDI(int noteNum, int value);
  void SendButtonMIDI(int noteNum, int value);

  /* MIDI handling */
  void Send_Volume(int channel, int value);
  void Send_Aftertouch(int channel, int noteNum, int value);
  void Send_ChannelPressure(int channel, int value);
  void Send_NoteOn(int channel, int noteNum, int value);
  void Send_NoteOff(int channel, int noteNum, int value);
  
  int m_padToNoteMap[MANTA_PADS];
  int m_sliderToNoteMap[MANTA_SLIDERPOS];
  int m_buttonToNoteMap[MANTA_BUTTONS];

  MidiNote m_padNotes[MAX_MIDI_NOTES];
  MidiNote m_buttonNotes[MAX_MIDI_NOTES];
};

#endif
