#include "MantaMidiSettings.h"
#include "settings/MantaSettingsParser.h"
#include <iostream>

using namespace std;

MantaMidiSettings::MantaMidiSettings()
{
    m_pSettingsParser = new MantaSettingsParser(this);
    strcpy(m_layoutPath, "");
    Reset();
}

MantaMidiSettings::MantaMidiSettings(int argc, char **argv)
{
    m_pSettingsParser = new MantaSettingsParser(this);
    strcpy(m_layoutPath, "");
    Reset();
}

MantaMidiSettings::~MantaMidiSettings()
{
    delete m_pSettingsParser;
}

bool MantaMidiSettings::LoadArgs(int argc, char **argv)
{
    bool bRet = true;
    bool bShowSettings = false;

    for (int i = 0; i < argc; ++i)
    {
        if ( 0 == strcmp(argv[i], "-d") )
            m_bDebugMode = true;
        else if ( 0 == strcmp(argv[i], "-v" ) )
            m_bUseVelocity = true;
        else if ( 0 == strcmp(argv[i], "-h") )
            bRet = false;
        else if ( 0 == strcmp(argv[i], "-layoutfile"))
        {
            if ( i + 1 < argc && argv[++i])
                strcpy(m_layoutPath, argv[i]);
        }
        else if (0 == strcmp(argv[i], "-showsettings") || 0 == strcmp(argv[i], "-V"))
            bShowSettings = true;
    }

    LoadSettings();

    if (bShowSettings)
        PrintSettings();

    return bRet;
}

void MantaMidiSettings::PrintUsage()
{
  printf("Usage: MantaMIDI [options]\n");
  printf("\n");
  printf("Options:\n");
  printf("\t-d : debug mode (prints Manta and MIDI events)\n");
  printf("\t-h : print usage / help\n");
  printf("\t-v : Use Velocity\n");
  printf("\t-layoutfile <filename> : Select the preferences file to use\n");
  printf("\t-showsettings, -V : Show the loaded settings at runtime {-V for verbose}\n");
  printf("\nExamples:\n");
  printf("\tMantaMIDI -layoutfile Hayden.txt\n");
}

/* Master settings */
bool MantaMidiSettings::GetDebugMode()  { return m_bDebugMode; }
bool MantaMidiSettings::GetUseVelocity() { return m_bUseVelocity; }
void MantaMidiSettings::SetUseVelocity(bool bUseVelocity) { m_bUseVelocity = bUseVelocity; }
void MantaMidiSettings::SetPadLayoutTitle(const char *strTitle) { strcpy(m_layoutTitle, strTitle); }
const char * MantaMidiSettings::GetPadLayoutTitle() { return m_layoutTitle; }

/* Pads */
unsigned char MantaMidiSettings::GetPad_EventChannel(int pad) {return m_padEventChannel[pad]; }
char MantaMidiSettings::GetPad_Midi(int pad) { return m_basePadMidi[pad]; }
char MantaMidiSettings::GetPad_MonoCCNumber() { return m_padMonoCCNumber; }
unsigned char MantaMidiSettings::GetPad_MaxVal(int pad) { return m_padMaxValue[pad]; }
unsigned char MantaMidiSettings::GetPad_LEDChannel(int pad) { return m_padLEDChannel[pad]; }
unsigned char MantaMidiSettings::GetPad_AmberLEDMidi(int pad) { return m_AmberLEDMidi[pad]; }
unsigned char MantaMidiSettings::GetPad_RedLEDMidi(int pad) { return m_RedLEDMidi[pad]; }
int MantaMidiSettings::GetPadFromMidi(unsigned char midiIn, Manta::LEDState &color)
{
    int iRet = -1;
    color = Manta::Off;

    for (int i = 0; i < numPads && iRet == -1; ++i)
    {
        if (m_AmberLEDMidi[i] == midiIn)
        {
            iRet = i;
            color = Manta::Amber;
        }
    }

    for (int i = 0; i < numPads && iRet == -1; ++i)
    {
        if (m_RedLEDMidi[i] == midiIn)
        {
            iRet = i;
            color = Manta::Red;
        }
    }

    return iRet;
}

Manta::LEDState MantaMidiSettings::GetPad_InactiveColor(int pad) { return m_inactivePadColor[pad]; }
Manta::LEDState MantaMidiSettings::GetPad_OnColor(int pad) { return m_onPadColor[pad]; }
Manta::LEDState MantaMidiSettings::GetPad_OffColor(int pad) { return m_offPadColor[pad]; }
PadValMode MantaMidiSettings::GetPad_Mode() { return m_padMode; }
void MantaMidiSettings::SetPad_Mode(PadValMode mode)
{
    m_padMode = mode;
}

void MantaMidiSettings::SetPad(int pad, unsigned char channel, unsigned char note)
{
    m_padEventChannel[pad] = channel;
    m_basePadMidi[pad] = note;
}

void MantaMidiSettings::SetPad_MIDINote(int pad, unsigned char note)
{
    m_basePadMidi[pad] = note;
}

void MantaMidiSettings::SetPad_Channel(unsigned char channel)
{
    for (int i = 0; i < numPads; i++)
    {
        m_padEventChannel[i] = channel;
    }
}

void  MantaMidiSettings::SetPad_MonoCCNumber(int ccnum)
{
    m_padMonoCCNumber = ccnum;
}

void MantaMidiSettings::CalibratePad(int pad, unsigned char value)
{
    if (m_padMaxValue[pad] < value)
        m_padMaxValue[pad] = value;
}

void MantaMidiSettings::SetPadLED_MidiChannel(int pad, unsigned char channel)
{
    m_padLEDChannel[pad] = channel;
}

void MantaMidiSettings::SetPadLED_AmberMidiNote(int pad, unsigned char amberNote)
{
    m_AmberLEDMidi[pad] = amberNote;
}

void MantaMidiSettings::SetPadLED_RedMidiNote(int pad, unsigned char redNote)
{
    m_RedLEDMidi[pad] = redNote;
}

void MantaMidiSettings::SetPadLED(int pad, unsigned char channel, unsigned char amberNote, unsigned char redNote)
{
    m_padLEDChannel[pad] = channel;
    m_AmberLEDMidi[pad] = amberNote;
    m_RedLEDMidi[pad] = redNote;
}

void MantaMidiSettings::SetAllPadOnColor(Manta::LEDState onColor)
{
    for (int i = 0; i < numPads; ++i)
        m_onPadColor[i] = onColor;
}

void MantaMidiSettings::SetAllPadOffColor(Manta::LEDState offColor)
{
    for (int i = 0; i < numPads; ++i)
        m_offPadColor[i] = offColor;
}

void MantaMidiSettings::SetAllPadInactiveColor(Manta::LEDState inactiveColor)
{
    for (int i = 0; i < numPads; ++i)
        m_inactivePadColor[i] = inactiveColor;
}

/* Sliders */
bool MantaMidiSettings::IsValidSliderIndex(int slider) { return slider == 0 || slider == 1; }
unsigned char MantaMidiSettings::GetSlider_EventChannel(int slider)
{
    if ( IsValidSliderIndex(slider))
        return m_sliderEventChannel[slider];
    else
        return 0;
}
void MantaMidiSettings::SetSlider_Channel(int slider, unsigned char channel)
{
    if ( IsValidSliderIndex(slider) )
        m_sliderEventChannel[slider] = channel;
}

char MantaMidiSettings::GetSlider_MidiNote(int slider)
{
    if ( IsValidSliderIndex(slider) )
        return m_sliderMidiNote[slider];
    else
        return -1;
}

void MantaMidiSettings::SetSlider_Midi(int slider, char midi)
{
    if ( IsValidSliderIndex(slider) )
        m_sliderMidiNote[slider] = midi;
}

unsigned short MantaMidiSettings::GetSlider_MaxVal(int slider)
{
    if ( IsValidSliderIndex(slider) )
        return m_sliderMaxValue[slider];
    else
        return 4096;
}

SliderMode MantaMidiSettings::GetSlider_Mode(int slider)
{
    if ( IsValidSliderIndex(slider) )
        return m_sliderMode[slider];
    else
        return smContinuous;
}
void MantaMidiSettings::SetSlider_Mode(int slider, SliderMode mode)
{
    if ( IsValidSliderIndex(slider) )
        m_sliderMode[slider] = mode;
}

void MantaMidiSettings::SetSlider(int slider, unsigned char channel, unsigned char note, SliderMode mode)
{
    m_sliderEventChannel[slider] = channel;
    m_sliderMidiNote[slider] = note;
    m_sliderMode[slider] = mode;
}

void MantaMidiSettings::CalibrateSlider(int slider, int value)
{
    if (m_sliderMaxValue[slider] < value)
        m_sliderMaxValue[slider] = value;
}

/* Buttons */
bool MantaMidiSettings::IsValidButtonIndex(int button) { return button >= 0 && button < numButtons; }
ButtonMode MantaMidiSettings::GetButton_Mode(int button)
{
    if ( IsValidButtonIndex(button) )
        return m_buttonMode[button];
    else
        return bmNote;
}

void MantaMidiSettings::SetButton_Mode(int button, ButtonMode mode)
{
    if ( IsValidButtonIndex(button) )
        m_buttonMode[button] = mode;
}

unsigned char MantaMidiSettings::GetButton_EventChannel(int button)
{
    if ( IsValidButtonIndex(button) )
        return m_buttonEventChannel[button];
    else
        return 0;
}
void MantaMidiSettings::SetButton_Channel(int button, unsigned char channel)
{
    if ( IsValidButtonIndex(button) && channel < 16)
        m_buttonEventChannel[button] = channel;
}

char MantaMidiSettings::GetButton_Midi(int button)
{
    if (IsValidButtonIndex(button))
        return m_buttonMidi[button];
    else
        return 0;
}
void MantaMidiSettings::SetButton_Midi(int button, char midi)
{
    if ( IsValidButtonIndex(button))
        m_buttonMidi[button] = midi;
}

unsigned char MantaMidiSettings::GetButton_MaxValue(int button)
{
    if ( IsValidButtonIndex(button))
        return m_buttonMaxValue[button];
    else
        return 255;
}

Manta::LEDState MantaMidiSettings::GetButton_OnColor(int button)
{
    if ( IsValidButtonIndex(button) )
        return m_onButtonColor[button];
    else
        return Manta::Off;
}
Manta::LEDState MantaMidiSettings::GetButton_OffColor(int button)
{
    if ( IsValidButtonIndex(button) )
        return m_offButtonColor[button];
    else
        return Manta::Off;
}
Manta::LEDState MantaMidiSettings::GetButton_InactiveColor(int button)
{
    if ( IsValidButtonIndex(button) )
        return m_inactiveButtonColor[button];
    else
        return Manta::Off;
}

void MantaMidiSettings::SetButton_OnColor(int button, Manta::LEDState color)
{
    if ( IsValidButtonIndex(button) )
        m_onButtonColor[button] = color;
}

void MantaMidiSettings::SetButton_OffColor(int button, Manta::LEDState color)
{
    if ( IsValidButtonIndex(button) )
        m_offButtonColor[button] = color;
}

void MantaMidiSettings::SetButton_InactiveColor(int button, Manta::LEDState color)
{
    if ( IsValidButtonIndex(button) )
        m_inactiveButtonColor[button] = color;
}


void MantaMidiSettings::SetButton(int button, unsigned char channel, unsigned char key, ButtonMode mode, Manta::LEDState onColor, Manta::LEDState offColor, Manta::LEDState inactiveColor)
{
    if ( IsValidButtonIndex(button) )
    {
        m_buttonMode[button] = mode;
        m_buttonEventChannel[button] = channel;
        m_buttonMidi[button] = key;
        m_onButtonColor[button] = onColor;
        m_offButtonColor[button] = offColor;
        m_inactiveButtonColor[button] = inactiveColor;
    }
}

void MantaMidiSettings::IncrementOctaveOffset()
{
    if (m_cOctaveOffset < 2)
        m_cOctaveOffset++;
}

void MantaMidiSettings::DecrementOctaveOffset()
{
    if (m_cOctaveOffset > -2)
        m_cOctaveOffset--;
}

char MantaMidiSettings::GetOctaveOffset()
{
    return m_cOctaveOffset;
}

char MantaMidiSettings::GetOctaveMidiOffset()
{
    return m_cOctaveOffset * 12;
}

void MantaMidiSettings::CalibrateButton(int button, int value)
{
    if (m_buttonMaxValue[button] < value)
        m_buttonMaxValue[button] = value;
}

void MantaMidiSettings::Reset()
{
    m_bDebugMode = false;
    m_cOctaveOffset = 0;

    for (int i = 0; i < numPads; ++i)
    {
        m_padMaxValue[i] = defaultMaxPadVal;
        m_inactivePadColor[i] = Manta::Off;
        m_onPadColor[i] = Manta::Amber;
        m_offPadColor[i] = Manta::Off;
    }
    for (int i = 0; i < numSliders; ++i)
        m_sliderMaxValue[i] = defaultMaxSliderVal;
    for (int i = 0; i < numButtons; ++i)
    {
        m_buttonMaxValue[i] = defaultMaxButtonVal;
        m_inactiveButtonColor[i] = Manta::Off;
        m_onButtonColor[i] = Manta::Amber;
        m_offButtonColor[i] = Manta::Off;
    }
}

void MantaMidiSettings::LoadSettings()
{
    if ( strlen(m_layoutPath) > 0)
    {
        m_pSettingsParser->ReadCollFile(m_layoutPath);

        printf("Using settings from %s.\n\n", m_layoutPath);
    }
    else
    {
        printf("Using default settings.\n");
        printf("To use custom settings, quit and re-run with \"-layoutfile <filename>\"\n\n");
    }

    m_pSettingsParser->UpdateSettings();
}

void MantaMidiSettings::PrintSettings()
{
    m_pSettingsParser->PrintSettings();
}
