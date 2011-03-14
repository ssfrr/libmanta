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
  // First Row
  m_padToNoteMap[0] = 32; // G#1
  m_padToNoteMap[1] = 35; // B1
  m_padToNoteMap[2] = 38; // D2
  m_padToNoteMap[3] = 53; // F3
  m_padToNoteMap[4] = 56; // G#3
  m_padToNoteMap[5] = 59; // B3
  m_padToNoteMap[6] = 62; // D4
  m_padToNoteMap[7] = 77; // F5

  // Second Row
  m_padToNoteMap[8] = 40; // E2
  m_padToNoteMap[9] = 43; // G2
  m_padToNoteMap[10] = 46; // A#2
  m_padToNoteMap[11] = 61; // C#4
  m_padToNoteMap[12] = 64; // E4
  m_padToNoteMap[13] = 67; // G4
  m_padToNoteMap[14] = 70; // A#4
  m_padToNoteMap[15] = 85; // C#6

  // Third Row
  m_padToNoteMap[16] = 24; // A1
  m_padToNoteMap[17] = 36; // C2
  m_padToNoteMap[18] = 39; // D#2
  m_padToNoteMap[19] = 54; // F#3
  m_padToNoteMap[20] = 57; // A3
  m_padToNoteMap[21] = 60; // C4
  m_padToNoteMap[22] = 75; // D#5
  m_padToNoteMap[23] = 78; // F#5

  // Fourth Row
  m_padToNoteMap[24] = 41; // F2
  m_padToNoteMap[25] = 44; // G#2
  m_padToNoteMap[26] = 47; // B2
  m_padToNoteMap[27] = 62; // D4
  m_padToNoteMap[28] = 65; // F4
  m_padToNoteMap[29] = 68; // G#4
  m_padToNoteMap[30] = 71; // B4
  m_padToNoteMap[31] = 86; // D6

  // Fifth Row
  m_padToNoteMap[32] = 34; // A#1
  m_padToNoteMap[33] = 37; // C#2
  m_padToNoteMap[34] = 52; // E3
  m_padToNoteMap[35] = 55; // G3
  m_padToNoteMap[36] = 58; // A#3
  m_padToNoteMap[37] = 73; // C#5
  m_padToNoteMap[38] = 76; // E5
  m_padToNoteMap[39] = 79; // G5

  // Sixth Row
  m_padToNoteMap[40] = 42; // F#2
  m_padToNoteMap[41] = 45; // A2
  m_padToNoteMap[42] = 48; // C3
  m_padToNoteMap[43] = 63; // D#4
  m_padToNoteMap[44] = 66; // F4
  m_padToNoteMap[45] = 69; // A4
  m_padToNoteMap[46] = 72; // C5
  m_padToNoteMap[47] = 87; // D#6

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
	{
	  Send_NoteOn(channel, midiNote, 100);
	  SetPadLED(Red, noteNum);
	}
      else if (value == 0)
	{
	  Send_NoteOff(channel, midiNote, 0);
	  SetPadLED(Off, noteNum);
	}
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
