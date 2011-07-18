#include "../MantaMidiSettings.h"
#include "MIDImanager.h"
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <math.h>

MidiManager::MidiManager(MantaMidiSettings *options) :
  m_options(options), m_padAftertouchStackIndex(-1)
{
}

MidiManager::~MidiManager()
{
}

void MidiManager::ResetLEDS()
{
    Initialize();
}

void MidiManager::Initialize()
{
    SetLEDControl(Manta::PadAndButton, true);
    //SetLEDControl(Manta::Slider, true);

    for(int i = 0; i < MANTA_BUTTONS; ++i)
    {
        if (-1 == (char)m_options->GetButton_Midi(i))
            SetButtonLED(m_options->GetButton_InactiveColor(i), i);
        else
            SetButtonLED(m_options->GetButton_OffColor(i), i);
    }

    for(int i = 0; i < MANTA_PADS; ++i)
    {
        if (-1 == (char)m_options->GetPad_Midi(i))
            SetPadLED(m_options->GetPad_InactiveColor(i), i);
        else
            SetPadLED(m_options->GetPad_OffColor(i), i);
    }
}

void MidiManager::PadEvent(int row, int column, int id, int value)
{
  if (m_options->GetDebugMode())
    std::cout << "PadEvent: " << id << ", " << value << "\n";

  if (!m_options->GetUseVelocity())
    SendPadMIDI(id, value);
}

void MidiManager::SliderEvent(int id, int value)
{
  if (m_options->GetDebugMode())
    std::cout << "SliderEvent: " << id << ", " << value << "\n";

    SendSliderMIDI(id, value);
}

void MidiManager::ButtonEvent(int id, int value)
{
  if (m_options->GetDebugMode())
    std::cout << "ButtonEvent: " << id << ", " << value << "\n";

  if (!m_options->GetUseVelocity())
    SendButtonMIDI(id, value);
}

void MidiManager::PadVelocityEvent(int row, int column, int id, int value)
{
  if (m_options->GetDebugMode())
    std::cout << "PadVelocityEvent: " << id << ", " << value << "\n";

  if (m_options->GetUseVelocity())
    SendPadMIDI(id, value);
}

void MidiManager::ButtonVelocityEvent(int id, int value)
{
  if (m_options->GetDebugMode())
    std::cout << "ButtonVelocityEvent: " << id << ", " << value << "\n";

  if (m_options->GetUseVelocity())
    SendButtonMIDI(id, value);
}

void MidiManager::SendPadMIDI(int noteNum, int value)
{
  int channel = m_options->GetPad_EventChannel(noteNum);
  int midiNote = m_options->GetPad_Midi(noteNum);

  if ((char)midiNote >= 0)
    {
      MidiNote &note = m_padNotes[midiNote];

      // Note On
      if (value > 0 && note.lastValue == 0)
	{
          if (m_options->GetUseVelocity())
	    Send_NoteOn(channel, midiNote, value);
	  else
	    Send_NoteOn(channel, midiNote, 100);

          SetPadLED(m_options->GetPad_OnColor(noteNum), noteNum);
	  
          if (m_options->GetPad_Mode() == pvmMonoAftertouch)
	    PushAftertouch(noteNum);
	}
      // Aftertouch
      else if (value > 0 && note.lastValue > 0)
	{
          if (m_options->GetPad_Mode() == pvmMonoAftertouch &&
	      IsCurrentPadMaximum(noteNum, value))
	    Send_Aftertouch(channel, midiNote, TranslatePadValueToMIDI(value));
          else if (m_options->GetPad_Mode() == pvmPolyAftertouch)
	    Send_Aftertouch(channel, midiNote, TranslatePadValueToMIDI(value));
          //else if (m_options->GetPadMode() == pvmPolyContinuous)
	    //Send_ControlChange(channel, midiNote, value);
	}
      else // Note Off
	{
	  Send_NoteOff(channel, midiNote, 0);

          SetPadLED(m_options->GetPad_OffColor(noteNum), noteNum);
	  
          if (m_options->GetPad_Mode() == pvmMonoAftertouch)
	    PopAftertouch(noteNum);
	}

      note.lastValue = value;
      note.curValue = value;
    }
}

bool MidiManager::IsCurrentPadMaximum(int noteNum, int value)
{
  bool bRet = false;
  int curMax = 0;
  
  // Loop through and get the maximum of the current pad values
  for(int i = 0; i <= m_padAftertouchStackIndex; ++i)
    {
      int midiNote = m_options->GetPad_Midi(m_padAftertouchStack[i]);
      MidiNote &note = m_padNotes[midiNote];
    
      if (note.curValue > curMax)
	curMax = note.curValue;
    }

  if (value > curMax)
    bRet = true;

  return bRet;
}

void MidiManager::SendSliderMIDI(int whichSlider, int value)
{
    int channel = m_options->GetSlider_EventChannel(whichSlider);
    char midiNote =  (char)m_options->GetSlider_MidiNote(whichSlider);

    if (value != 0x0000FFFF)
      Send_ControlChange(channel, midiNote, TranslateSliderValueToCC(value));
}

int MidiManager::TranslatePadValueToMIDI(int padValue)
{
  int iRet = 0;
  double transVal = (127.0 / 210.0);

  int tempPadValue = padValue;
  if (tempPadValue > 210)
    tempPadValue = 210;
  
  iRet = (int)(round(padValue * transVal));
  
  return iRet;
}

int MidiManager::TranslateSliderValueToCC(int sliderValue)
{
  int iRet = 0;
  double transVal = (127.0 / 4096.0);

  iRet = (int)(round(sliderValue * transVal));

  return iRet;
}

void MidiManager::SendButtonMIDI(int noteNum, int value)
{
  int channel = m_options->GetButton_EventChannel(noteNum);
  int midiNote = m_options->GetButton_Midi(noteNum);

  if ((char)midiNote >= 0)
    {
      MidiNote &note = m_buttonNotes[midiNote];
      
      if (m_options->GetUseVelocity())
	{
	  Send_NoteOn(channel, midiNote, value);
	  if (value > 0) 
            SetButtonLED(m_options->GetButton_OnColor(noteNum), noteNum);
	  else // value == 0
            SetButtonLED(m_options->GetButton_OffColor(noteNum), noteNum);
	}
      else
	{
	  if (0 == note.lastValue && value > 0)
	    {
	      Send_NoteOn(channel, midiNote, 100);
              SetButtonLED(m_options->GetButton_OnColor(noteNum), noteNum);
	    }
	  else if (value == 0)
	    {
	      Send_NoteOff(channel, midiNote, 0);
              SetButtonLED(m_options->GetButton_OffColor(noteNum), noteNum);
	    }
	}
      
      note.lastValue = value;
    }
}

void MidiManager::Send_NoteOff(int channel, int noteNum, int velocity)
{
  if ( (channel >= 0 && channel < 16) && (noteNum >= 0 && noteNum < 128) )
    SendMIDI( channel, atNoteOff, noteNum, velocity );
}

void MidiManager::Send_NoteOn(int channel, int noteNum, int velocity)
{
  if ( (channel >= 0 && channel < 16) && (noteNum >= 0 && noteNum < 128) )
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

void MidiManager::PushAftertouch(int key)
{ 
  m_padAftertouchStack[++m_padAftertouchStackIndex] = key;
}

void MidiManager::PopAftertouch(int key)
{
  bool bFound = false;
  for (int i = 0; i <= m_padAftertouchStackIndex; ++i)
    {
      if (m_padAftertouchStack[i] == key)
	  bFound = true;
      
      if (bFound)
	{
	  if (i+1 <= m_padAftertouchStackIndex)
	    m_padAftertouchStack[i] = m_padAftertouchStack[i+1];
	}
    }
  --m_padAftertouchStackIndex;
}

void MidiManager::SendMIDI(unsigned char ucChannel, MidiActionType actionType, int noteNum, int value)
{
  unsigned char data[3];
  int nBytes = 0;

  if (value > 127)
    value &= 0x7F;
  
  // Note Off: 128, Note, Velocity
  if (actionType == atNoteOff)
    {
      data[0] = 0x80 + ucChannel; // 128
      data[1] = noteNum;
      data[2] = value;
      nBytes = 3;
      
      if (m_options->GetDebugMode())
	printf("Note off: %d %d %d\n", data[0], data[1], data[2]);
    }
  // Note On: 144/0x90, Note, Velocity
  else if(actionType == atNoteOn)
    {
      data[0] = 0x90 + ucChannel; // 144
      data[1] = noteNum;
      data[2] = value;
      nBytes = 3;
      
      if (m_options->GetDebugMode())
	printf("Note on: %d %d %d\n", data[0], data[1], data[2]);
    }
  // Polyphonic Pressure (Aftertouch): 0xA0, controller # (0-119), value
  else if (actionType == atPolyphonicKeyPressure)
    {
      data[0] = 0xA0 + ucChannel; //
      data[1] = noteNum;
      data[2] = value;
      nBytes = 3;
      
      if (m_options->GetDebugMode())
	printf("Polyphonic Pressure: %d %d %d\n", data[0], data[1], data[2]);
    }
  // Control Change: 0xB0
  else if (actionType == atControlChange)
    {
      data[0] = 0xB0 + ucChannel;
      data[1] = noteNum;
      data[2] = value;
      nBytes = 3;
      
      if (m_options->GetDebugMode())
	printf("Program Change: %d %d %d\n", data[0], data[1], data[2]);
    }
  // Program Change: 0xC0
  else if (actionType == atProgramChange)
    {
      data[0] = 0xC0 + ucChannel;
      data[1] = noteNum;
      nBytes = 2;
      
      if (m_options->GetDebugMode())
	printf("Program Change: %d %d\n", data[0], data[1]);
    }
  else if (actionType == atChannelPressure)
    {
      if (m_options->GetDebugMode())
	printf("Channel Pressure (aftertouch)\n");
    }
  else if (actionType == atPitchWheel)
    {
      if (m_options->GetDebugMode())
	printf("Pitch Wheel Change: \n");
    }
  
  SendMIDI(data, nBytes);
}
