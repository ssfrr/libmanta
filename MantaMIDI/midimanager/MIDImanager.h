#ifndef _MIDIMANAGER_H
#define _MIDIMANAGER_H

#include "../../core/Manta.h"
#include "../../core/MantaExceptions.h"
#include <stdint.h>

#define MANTA_PADS      48
#define MANTA_SLIDERPOS 127
#define MANTA_BUTTONS   4
#define MAX_MIDI_NOTES	128		// 48 for the actual manta array...

class MantaMidiSettings;

enum MidiActionType
  {
    atNoteOff = 0,
    atNoteOn = 1,
    atPolyphonicKeyPressure,
    atControlChange,
    atProgramChange,
    atChannelPressure,
    atPitchWheel
  };

typedef struct
{
  int timeOn;
  int lastValue;
  int curValue;
} MidiNote;

class MidiManager : public Manta
{
 public:
  MidiManager(MantaMidiSettings *options);
  ~MidiManager();

  void Initialize();
  void ResetLEDS();
  void SetCalibrateMode(bool bCalModeOn);
  bool GetCalibrationState();

  MantaMidiSettings *GetOptions();
  
 protected:
  MantaMidiSettings *m_options;

  virtual void InitializeMIDI() = 0;
  virtual void SendMIDI(unsigned char data[], int nBytes) = 0;
  
 private:

  /* Manta events */
  virtual void PadEvent(int row, int column, int id, int value);
  virtual void SliderEvent(int id, int value);
  virtual void ButtonEvent(int id, int value);
  virtual void PadVelocityEvent(int row, int column, int id, int value);
  virtual void ButtonVelocityEvent(int id, int value);

  void SendMIDI(unsigned char ucChannel, MidiActionType actionType, int noteNum, int value);

  int TranslatePadValueToMIDI(int padValue);
  int TranslateSliderValueToCC(int sliderValue);

  /* Handling functions */
  void SendPadMIDI(int noteNum, int value);
  void SendSliderMIDI(int whichSlider, int value);
  void SendButtonMIDI(int noteNum, int value);

  /* MIDI handling */
  void Send_NoteOn(int channel, int noteNum, int value);
  void Send_NoteOff(int channel, int noteNum, int value);
  void Send_Aftertouch(int channel, int noteNum, int value);
  void Send_ControlChange(int channel, int controller, int value);
  void Send_ProgramChange(int channel, int newValue);
  void Send_ChannelPressure(int channel, int value);
  void Send_PitchWheelChange(int channel, int value);

  void PushAftertouch(int key);
  void PopAftertouch(int key);
  bool IsCurrentPadMaximum(int noteNum, int value);

  MidiNote m_padNotes[MAX_MIDI_NOTES];
  MidiNote m_buttonNotes[MAX_MIDI_NOTES];

  int m_padAftertouchStack[MANTA_PADS];
  int m_padAftertouchStackIndex;

  bool m_bCalibrationMode;
};

#endif
