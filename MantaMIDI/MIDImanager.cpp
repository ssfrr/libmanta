#include "OptionHolder.h"
#include "MIDImanager.h"
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <math.h>

MidiManager::MidiManager(OptionHolder &options) :
  m_options(options)
{
  InitializeMapValues();
}

MidiManager::~MidiManager()
{
	
}

void MidiManager::PadEvent(int id, int value)
{
  if (m_options.GetDebugMode())
    std::cout << "PadEvent: " << id << ", " << value << "\n";

  if (!m_options.GetUseVelocity())
    SendPadMIDI(id, value);
}

void MidiManager::SliderEvent(int id, int value)
{
  if (m_options.GetDebugMode())
    std::cout << "SliderEvent: " << id << ", " << value << "\n";

    SendSliderMIDI(id, value);
}

void MidiManager::ButtonEvent(int id, int value)
{
  if (m_options.GetDebugMode())
    std::cout << "ButtonEvent: " << id << ", " << value << "\n";

  if (!m_options.GetUseVelocity())
    SendButtonMIDI(id, value);
}

void MidiManager::PadVelocityEvent(int id, int value)
{
  if (m_options.GetDebugMode())
    std::cout << "PadVelocityEvent: " << id << ", " << value << "\n";

  if (m_options.GetUseVelocity())
    SendPadMIDI(id, value);
}

void MidiManager::ButtonVelocityEvent(int id, int value)
{
  if (m_options.GetDebugMode())
    std::cout << "ButtonVelocityEvent: " << id << ", " << value << "\n";

  if (m_options.GetUseVelocity())
    SendButtonMIDI(id, value);
}

void MidiManager::InitializeMapValues()
{
  switch(m_options.GetPadLayout())
    {
    case plPiano:
      AssignPianoLayout();
      break;
    case plChromatic:
      AssignChromaticLayout();
      break;
    case plHoneycomb:
      AssignHoneycombLayout();
    default:
      break;
    }
  
  for(int i = 0; i < MANTA_BUTTONS; ++i)
    m_buttonToNoteMap[i] = m_options.GetBaseButtonMidi() + i;
}

void MidiManager::AssignPianoLayout()
{
  int baseMidi = m_options.GetBasePadMidi();
  int padIndex = 0;
  for (int i = 0; i < (MANTA_PAD_ROWS / 2); ++i)
    {
      m_padToNoteMap[padIndex] = baseMidi++;     // 0
      m_padToNoteMap[padIndex + 8] = baseMidi++; // 1
      padIndex++;
      m_padToNoteMap[padIndex] = baseMidi++;     // 2
      m_padToNoteMap[padIndex + 8] = baseMidi++; // 3
      padIndex++;
      m_padToNoteMap[padIndex] = baseMidi++;     // 4
      m_padToNoteMap[padIndex + 8] = -1;
      padIndex++;
      m_padToNoteMap[padIndex] = baseMidi++;     // 5
      m_padToNoteMap[padIndex + 8] = baseMidi++; // 6
      padIndex++;
      m_padToNoteMap[padIndex] = baseMidi++;     // 7
      m_padToNoteMap[padIndex + 8] = baseMidi++; // 8
      padIndex++;
      m_padToNoteMap[padIndex] = baseMidi++;     // 9
      m_padToNoteMap[padIndex + 8] = baseMidi++; // 10
      padIndex++;
      m_padToNoteMap[padIndex] = baseMidi++;     // 11
      m_padToNoteMap[padIndex + 8] = -1;
      padIndex++;
      m_padToNoteMap[padIndex] = baseMidi;       //12
      m_padToNoteMap[padIndex + 8] = -1;
      padIndex += 9;
    }
}

void MidiManager::AssignChromaticLayout()
{
  for(int i = 0; i < MANTA_PADS; ++i)
    m_padToNoteMap[i] = m_options.GetBasePadMidi() + i;
}

void MidiManager::AssignHoneycombLayout()
{
}

void MidiManager::SendPadMIDI(int noteNum, int value)
{
  int channel = m_options.GetPadEventChannel();
  int midiNote = m_padToNoteMap[noteNum];
  MidiNote &note = m_padNotes[midiNote];

  if (m_options.GetUseVelocity())
    Send_NoteOn(channel, midiNote, value);
  else
    {
      if (0 == note.lastValue && value > 0)
	Send_NoteOn(channel, midiNote, 100);
      else if (value == 0)
	Send_NoteOff(channel, midiNote, 0);
    }

  note.lastValue = note.value;
  note.value = value;
}

void MidiManager::SendSliderMIDI(int whichSlider, int value)
{
  int channel;
  int midiNote;

  if (whichSlider == 0)
    {
      channel = m_options.GetSlider0_EventChannel();
      midiNote = m_options.GetSlider0_MidiNote();
    }
  else
    {
      channel = m_options.GetSlider1_EventChannel();
      midiNote = m_options.GetSlider1_MidiNote();
    }

  if (value != 0x0000FFFF)
      Send_ControlChange(channel, midiNote, TranslateSliderValueToCC(value));
}

int MidiManager::TranslateSliderValueToCC(int sliderValue)
{
  int iRet = 0;
  double transVal = (127.0 / 4096.0);

  iRet = (int)(round(sliderValue * transVal));
  //printf("Translate: %d %d\n", sliderValue, iRet);

  return iRet;
}

void MidiManager::SendButtonMIDI(int noteNum, int value)
{
  int channel = m_options.GetButtonEventChannel();
  int midiNote = m_buttonToNoteMap[noteNum];
  MidiNote &note = m_buttonNotes[midiNote];

  if (m_options.GetUseVelocity())
    Send_NoteOn(channel, midiNote, value);
  else
    {
      if (0 == note.lastValue && value > 0)
	Send_NoteOn(channel, midiNote, 100);
      else if (value == 0)
	Send_NoteOff(channel, midiNote, 0);
    }

  note.lastValue = note.value;
  note.value = value;
}

void MidiManager::Send_NoteOff(int channel, int noteNum, int velocity)
{
  if ( (channel > 0 && channel <= 16) && (noteNum >= 0 && noteNum < 128) )
    SendMIDI( channel, atNoteOff, noteNum, velocity );
}

void MidiManager::Send_NoteOn(int channel, int noteNum, int velocity)
{
  if ( (channel > 0 && channel <= 16) && (noteNum >= 0 && noteNum < 128) )
    SendMIDI( channel, atNoteOn, noteNum, velocity );
}

void MidiManager::Send_Aftertouch(int channel, int noteNum, int value)
{
  SendMIDI( channel, atPolyphonicKeyPressure, noteNum, value );
}

void MidiManager::Send_ControlChange(int channel, int controller, int value)
{
  SendMIDI( channel, atControlChange, controller, value);
}

void MidiManager::Send_ProgramChange(int channel, int newValue)
{
  SendMIDI( channel, atProgramChange, newValue, -1);
}

void MidiManager::Send_ChannelPressure(int channel, int value)
{
  SendMIDI( channel, atChannelPressure, value, -1 );
}

void MidiManager::Send_PitchWheelChange(int channel, int value)
{
  int sevenBitAnd = 0x0000007F;
  if ( value < 0x00003FFF )
    {
      unsigned char leastByte = value & sevenBitAnd;
      unsigned char mostByte = (value >> 7) & sevenBitAnd;
      SendMIDI( channel, atPitchWheel, leastByte, mostByte);
    }
}
