#include "../settings/MantaMidiSettings.h"
#include "MIDImanager.h"
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <math.h>

MidiManager::MidiManager(MantaMidiSettings *options) :
  m_options(options), m_padAftertouchStackIndex(-1), m_bCalibrationMode(false)
{
    for(int i = 0; i < MAX_MIDI_NOTES; ++i)
    {
        m_padNotes[i].lastValue = 0;
        m_buttonNotes[i].lastValue = 0;
    }
    for(int i = 0; i < MANTA_PADS; ++i)
    {
        m_padValues[i] = 0;
    }
    for(int i = 0; i < MANTA_BUTTONS; ++i)
    {
        m_buttonValues[i] = 0;
    }
    ResetLEDS();
}

MidiManager::~MidiManager()
{
}

void MidiManager::ResetLEDS()
{
    Initialize();
}

void MidiManager::SetCalibrateMode(bool bCalModeOn)
{
    m_bCalibrationMode = bCalModeOn;
}

bool MidiManager::GetCalibrationState()
{
    return m_bCalibrationMode;
}

void MidiManager::Initialize()
{
    if (m_options->GetDebugMode()) cout << "MidiManger::Initialize() Start" << endl;

    SetLEDControl(Manta::PadAndButton, true);
    SetLEDControl(Manta::Slider, false);

    for(int i = 0; i < MANTA_BUTTONS; ++i)
    {
        if (-1 == (char)m_options->GetButton_Midi(i))
        {
            if (m_options->GetDebugMode()) cout << "[Off] Set Button Color" << i << " " << m_options->GetButton_InactiveColor(i) << endl;
            SetButtonLED(m_options->GetButton_InactiveColor(i), i);
        }
        else
        {
            if (m_options->GetDebugMode()) cout << "Set Button Color" << i << " " << m_options->GetButton_OffColor(i) << endl;
            SetButtonLED(m_options->GetButton_OffColor(i), i);
        }
    }

    for(int i = 0; i < MANTA_PADS; ++i)
    {
        if (-1 == (char)m_options->GetPad_Midi(i))
        {
            SetPadLED(m_options->GetPad_InactiveColor(i), i);
            if (m_options->GetDebugMode()) cout << "[Off] Set Pad Color [" << (int)m_options->GetPad_Midi(i) << "] " << i << " " << m_options->GetPad_InactiveColor(i) << endl;
        }
        else
        {
            SetPadLED(m_options->GetPad_OffColor(i), i);
            if (m_options->GetDebugMode()) cout << "Set Pad Color [" << (int)m_options->GetPad_Midi(i) << "] " << i << " " << m_options->GetPad_OffColor(i) << endl;
        }
    }

    if (m_options->GetDebugMode()) cout << "MidiManger::Initialize() End" << endl;
}

MantaMidiSettings *MidiManager::GetOptions()
{
    return m_options;
}

void MidiManager::PadEvent(int row, int column, int id, int value)
{
  if (m_options->GetDebugMode()  && !m_options->GetUseVelocity())
    std::cout << "PadEvent: " << id << ", " << value << "\n";


  /*if (!GetCalibrationState())
  {*/
    SetPadValue(id, value);
    SendPadMIDI(id, value, false);
  /*}
  else
      m_options->CalibratePad(id, value);*/
}

void MidiManager::SliderEvent(int id, int value)
{
  if (m_options->GetDebugMode())
    std::cout << "SliderEvent: " << id << ", " << value << "\n";

    //if (!GetCalibrationState())
        SendSliderMIDI(id, value);
    /*else
         m_options->CalibrateSlider(id, value);*/
}

void MidiManager::ButtonEvent(int id, int value)
{
  if (m_options->GetDebugMode())
    std::cout << "ButtonEvent: " << id << ", " << value << "\n";

  /*if (!GetCalibrationState())
  {*/
  SetButtonValue(id, value);
  if (m_options->GetButton_Mode(id) == bmOctaveDecrement)
  {
    m_options->DecrementOctaveOffset();
    UpdateOffsetLEDs();
  }
  else if (m_options->GetButton_Mode(id) == bmOctaveIncrement)
  {
    m_options->IncrementOctaveOffset();
    UpdateOffsetLEDs();
  }
  else
    SendButtonMIDI(id, value, false);
  /*}
  else
    m_options->CalibrateButton(id, value);*/
}

void MidiManager::PadVelocityEvent(int row, int column, int id, int value)
{
  if (m_options->GetDebugMode() && m_options->GetUseVelocity())
    std::cout << "PadVelocityEvent: " << id << ", " << value << "\n";

    SendPadMIDI(id, value, true);
}

void MidiManager::ButtonVelocityEvent(int id, int value)
{
  if (m_options->GetDebugMode() && m_options->GetUseVelocity())
    std::cout << "ButtonVelocityEvent: " << id << ", " << value << "\n";

    SendButtonMIDI(id, value, true);
}

void MidiManager::SendPadMIDI(int noteNum, int value, bool bVelocityEvent)
{
  int channel = m_options->GetPad_EventChannel(noteNum);
  int midiNote = m_options->GetPad_Midi(noteNum);

  if ((char)midiNote >= 0)
    {
      MidiNote &note = m_padNotes[midiNote];

        // note on
        if (value > 0)
        {
            // Velocity Mode is on, and we got a velocity event, so we know it's a NoteOn or NoteOff
            if (m_options->GetUseVelocity() && bVelocityEvent)
            {
                SendPadNoteOn(channel, midiNote + m_options->GetOctaveMidiOffset(), noteNum, value);
            }
            // not a note on or note off, but a positive value; therefore aftertouch
            else if (m_options->GetUseVelocity() && !bVelocityEvent)
            {
                SendPadAftertouch(channel, midiNote + m_options->GetOctaveMidiOffset(), noteNum, value);
            }
            else // velocity mode is off
            {
                // Note On
                if (note.lastValue == 0 && !bVelocityEvent)
                {
                    SendPadNoteOn(channel, midiNote + m_options->GetOctaveMidiOffset(), noteNum, 100);
                    SendPadAftertouch(channel, midiNote + m_options->GetOctaveMidiOffset(), noteNum, value);
                }
                // Aftertouch
                else if (note.lastValue > 0)
                {
                   SendPadAftertouch(channel, midiNote + m_options->GetOctaveMidiOffset(), noteNum, value);
                }
            }
        }
        else // note off
        {
            // we're just ignoring velocity events with velocity zero to avoid redundant values
            // (because padevent is already sending them)
            if (!bVelocityEvent)
                SendPadNoteOff(channel, midiNote + m_options->GetOctaveMidiOffset(), noteNum);
        }

      note.lastValue = note.curValue;
      note.curValue = value;
    }
}

void MidiManager::SendPadNoteOn(int channel, int midiNote, int noteNum, int value)
{
    Send_NoteOn(channel, midiNote, value);

    SetPadLED(m_options->GetPad_OnColor(noteNum), noteNum);

    if (m_options->GetPad_Mode() == pvmMonoContinuous)
        PushAftertouch(noteNum);
}

void MidiManager::SendPadAftertouch(int channel, int midiNote, int noteNum, int value)
{
    if (m_options->GetPad_Mode() == pvmMonoContinuous && IsCurrentPadMaximum(value))
    {
       if (m_options->GetDebugMode()) cout << "MonoAftertouch: ";

       Send_ControlChange(channel, m_options->GetPad_MonoCCNumber(), TranslatePadValueToMIDI(noteNum, value));
    }
    else if (m_options->GetPad_Mode() == pvmPolyAftertouch)
    {
       if (m_options->GetDebugMode()) cout << "PolyAftertouch: ";

       Send_Aftertouch(channel, midiNote, TranslatePadValueToMIDI(noteNum, value));
    }
    else if (m_options->GetPad_Mode() == pvmPolyContinuous)
    {
       if (m_options->GetDebugMode()) cout << "PolyContinuous: ";

       Send_ControlChange(channel, midiNote, TranslatePadValueToMIDI(noteNum, value));
    }
}

void MidiManager::SendPadNoteOff(int channel, int midiNote, int noteNum)
{
    Send_NoteOff(channel, midiNote, 0);

    SetPadLED(m_options->GetPad_OffColor(noteNum), noteNum);

    if (m_options->GetPad_Mode() == pvmMonoContinuous)
        PopAftertouch(noteNum);

    SendPadAftertouch(channel, midiNote, noteNum, 0);
}

bool MidiManager::IsCurrentPadMaximum(int value)
{
  bool bRet = false;
  int curMax = 0;
  
  // Loop through and get the maximum of the current pad values
  for(int i = 0; i <= m_padAftertouchStackIndex; ++i)
    {
      int padValue = GetPadValue(m_padAftertouchStack[i]);
    
      if (padValue > curMax)
        curMax = padValue;
    }

  if (value >= curMax)
    bRet = true;

  return bRet;
}

void MidiManager::UpdateOffsetLEDs()
{
    for(int i=0; i < 4; ++i)
    {
        if (m_options->GetButton_Mode(i) == bmOctaveDecrement)
            SetNegOffsetColor(i);
        else if (m_options->GetButton_Mode(i) == bmOctaveIncrement)
            SetPosOffsetColor(i);
    }
}

void MidiManager::SetPosOffsetColor(int button)
{
    switch(m_options->GetOctaveOffset())
    {
    case 2: SetButtonLED(Manta::Red, button);
    case 1: SetButtonLED(Manta::Amber, button);
    default: SetButtonLED(Manta::Off, button);
    }
}

void MidiManager::SetNegOffsetColor(int button)
{
    m_options->DecrementOctaveOffset();
    switch(m_options->GetOctaveOffset())
    {
    case -2: SetButtonLED(Manta::Red, button);
    case -1: SetButtonLED(Manta::Amber, button);
    default: SetButtonLED(Manta::Off, button);
    }
}

void MidiManager::SendSliderMIDI(int whichSlider, int value)
{
    int channel = m_options->GetSlider_EventChannel(whichSlider);
    char midiNote =  (char)m_options->GetSlider_MidiNote(whichSlider);

    if (value != 0x0000FFFF)
      Send_ControlChange(channel, midiNote, TranslateSliderValueToCC(whichSlider, value));
}

int MidiManager::TranslatePadValueToMIDI(int pad, int padValue)
{
  int iRet = 0;
  unsigned char maxVal = m_options->GetPad_MaxVal(pad);
  double transVal = (127.0 / maxVal);

  int tempPadValue = padValue;
  if (tempPadValue > maxVal)
    tempPadValue = maxVal;
  
  iRet = (int)(round(tempPadValue * transVal));
  
  return iRet;
}

int MidiManager::TranslateSliderValueToCC(int slider, int sliderValue)
{
  int iRet = 0;
  unsigned short maxVal = m_options->GetSlider_MaxVal(slider);
  double transVal = (127.0 / maxVal);

  iRet = (int)(round(sliderValue * transVal));

  return iRet;
}

int MidiManager::TranslateButtonValueToMIDI(int button, int buttonValue)
{
    int iRet = 0;
    unsigned char maxVal = m_options->GetButton_MaxValue(button);
    double transVal = 127.0 / maxVal;

    int tempButtonVal = buttonValue;
    if (tempButtonVal > maxVal)
        tempButtonVal = maxVal;

    iRet = (int)(round(tempButtonVal * transVal));

    return iRet;
}

void MidiManager::SendButtonMIDI(int noteNum, int value, bool bVelocityEvent)
{
  int channel = m_options->GetButton_EventChannel(noteNum);
  int midiNote = m_options->GetButton_Midi(noteNum);

  if ((char)midiNote >= 0)
    {
      MidiNote &note = m_buttonNotes[midiNote];
      
      if (value > 0)
      {
          // velocity mode on - it's a noteon or noteoff
          if (m_options->GetUseVelocity() && bVelocityEvent)
              SendButtonNoteOn(channel, midiNote, noteNum, value);
          // not note on or note off, but positive, so it's aftertouch
          else if (m_options->GetUseVelocity() && !bVelocityEvent)
              SendButtonAftertouch(channel, midiNote, noteNum, value);
          else // velocity off
          {
              // Note On
              if (note.lastValue == 0 && !bVelocityEvent)
              {
                  SendButtonNoteOn(channel, midiNote, noteNum, 100);
                  SendButtonAftertouch(channel, midiNote, noteNum, value);
              }
              // Aftertouch
              else if (note.lastValue > 0)
                  SendButtonAftertouch(channel, midiNote, noteNum, value);
          }
      }
      else // note off
      {
          // ignore velocity events 'cause they're redundant
          if (!bVelocityEvent)
              SendButtonNoteOff(channel, midiNote, noteNum);
      }

      note.lastValue = value;
      note.curValue = value;
    }
}

void MidiManager::SendButtonNoteOn(int channel, int midiNote, int noteNum, int value)
{
    Send_NoteOn(channel, midiNote, value);

    SetButtonLED(m_options->GetButton_OnColor(noteNum), noteNum);
}

void MidiManager::SendButtonAftertouch(int channel, int midiNote, int noteNum, int value)
{
    if (m_options->GetButton_Mode(noteNum) == bmNote)
    {
       if (m_options->GetDebugMode()) cout << "Button Note: ";

       Send_Aftertouch(channel, midiNote, TranslateButtonValueToMIDI(noteNum, value));
    }
    else if (m_options->GetButton_Mode(noteNum) == bmController)
    {
       if (m_options->GetDebugMode()) cout << "Button CC: ";

       Send_ControlChange(channel, midiNote, TranslateButtonValueToMIDI(noteNum, value));
    }
}

void MidiManager::SendButtonNoteOff(int channel, int midiNote, int noteNum)
{
    Send_NoteOff(channel, midiNote, 0);
    SetButtonLED(m_options->GetButton_OffColor(noteNum), noteNum);
    SendButtonAftertouch(channel, midiNote, noteNum, 0);
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

void MidiManager:: PushAftertouch(int key)
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

