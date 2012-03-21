#include "MantaMidiSettings.h"
#include <iostream>

using namespace std;

MantaMidiSettings::MantaMidiSettings()
{
    Reset();
}

MantaMidiSettings::MantaMidiSettings(int argc, char **argv)
{
    Reset();
}

MantaMidiSettings::~MantaMidiSettings()
{
}

bool MantaMidiSettings::LoadArgs(int argc, char **argv)
{
    bool bRet = true;

    for (int i = 0; i < argc; ++i)
    {
     if ( 0 == strcmp(argv[i], "-d") )
        m_bDebugMode = true;
     else if ( 0 == strcmp(argv[i], "-v" ) )
        m_bUseVelocity = true;
     else if ( 0 == strcmp(argv[i], "-h") )
         bRet = false;
     else if ( 0 == strcmp(argv[i], "-padlayout") )
       {
         if ( i + 1 < argc && argv[++i])
           {
             switch(argv[i][0])
               {
               case 'P': SetPad_Layout(plPiano);
                 break;
               case 'C': SetPad_Layout(plChromatic);
                 break;
               case 'D': SetPad_Layout(plDuet);
                 break;
               default: SetPad_Layout(plHoneycomb);
                 break;
               }
           }
       }
     else if ( 0 == strcmp(argv[i], "-padmode") )
       {
         if ( i + 1 < argc && argv[++i])
           {
             switch(argv[i][0])
               {
               case '1':
                 m_padMode = pvmMonoContinuous;
                 break;
               case '2':
                 m_padMode = pvmPolyAftertouch;
                 break;
               default:
                 m_padMode = pvmPolyContinuous;
                 break;
               }
           }
       }
    }

    if (bRet)
        PrintOptionStatus();

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
  printf("\t-padlayout [layout] : Set Pad Layout\n");
  printf("\t\tLayouts\n");
  printf("\t\tP : Piano\n");
  printf("\t\tC : Chromatic\n");
  printf("\t\tD : Hayden Duet (default)\n");
  printf("\t\tH : Honeycomb\n");
  printf("\t-padmode [mode] : Set Pad Mode\n");
  printf("\t\tModes\n");
  printf("\t\t1 : Monophonic Continuous (default)\n");
  printf("\t\t2 : Polyphonic Aftertouch\n");
  printf("\t\t3 : Polyphonic Continuous\n");
  printf("\n");
  printf("Examples:\n\n");
  printf("\tMantaMIDI -padlayout D -padmode 2\n");
  printf("\tMantaMIDI -v -padlayout P -padmode 3\n");
}

/* Master settings */
bool MantaMidiSettings::GetDebugMode()  { return m_bDebugMode; }
bool MantaMidiSettings::GetUseVelocity() { return m_bUseVelocity; }
void MantaMidiSettings::SetUseVelocity(bool bUseVelocity) { m_bUseVelocity = bUseVelocity; }

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
PadLayout MantaMidiSettings::GetPad_Layout() { return m_padLayout; }
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

void MantaMidiSettings::CalibrateButton(int button, int value)
{
    if (m_buttonMaxValue[button] < value)
        m_buttonMaxValue[button] = value;
}

void MantaMidiSettings::PrintOptionStatus()
{
    cout << "Settings Initialized.\n";
    cout << "Pad Layout:";
    switch(GetPad_Layout())
    {
        case plHoneycomb: cout << "honeycomb" << endl; break;
        case plPiano: cout << "piano" << endl; break;
        case plChromatic: cout << "chromatic" << endl; break;
        case plDuet: cout << "duet" << endl; break;
        default:
            break;
    }

    /*printf("Debug Mode: %d\n", (int)m_bDebugMode);
    printf("Velocity: %d\n\n", (int)m_bUseVelocity);

    printf("Pad Event Channel: %d\n", m_padEventChannel);
    printf("Base Pad MIDI: %d\n", m_basePadMidi);
    printf("Pad LED Channel: %d\n", m_padLEDChannel);
    printf("Pad Layout: %d\n", m_padLayout);
    printf("Pad Mode: %d\n\n", m_padMode);

    printf("Slider 0 Event Channel: %d\n", m_slider0_EventChannel);
    printf("Slider 0 Midi Note: %d\n", m_slider0_MidiNote);
    printf("Slider 0 Mode: %d\n\n", m_slider0_Mode);
    printf("Slider 1 Event Channel: %d\n", m_slider1_EventChannel);
    printf("Slider 1 Event Channel: %d\n", m_slider1_MidiNote);
    printf("Slider 1 Mode: %d\n\n", m_slider1_Mode);

    printf("Button %d Event Channel: %d\n", 1, m_buttonEventChannel[0]);
    printf("Button %d Event Channel: %d\n", 2, m_buttonEventChannel[1]);
    printf("Button %d Event Channel: %d\n", 3, m_buttonEventChannel[2]);
    printf("Button %d Event Channel: %d\n", 4, m_buttonEventChannel[3]);
    printf("Button %d Mode: %d\n\n", 1, m_buttonMode[0]);
    printf("Button %d Mode: %d\n\n", 2, m_buttonMode[1]);
    printf("Button %d Mode: %d\n\n", 3, m_buttonMode[2]);
    printf("Button %d Mode: %d\n\n", 4, m_buttonMode[3]);*/
}

void MantaMidiSettings::Reset()
{
    m_bDebugMode = false;
    m_bUseVelocity = false;

    for (int i = 0; i < numPads; ++i)
        m_padMaxValue[i] = defaultMaxPadVal;
    for (int i = 0; i < numSliders; ++i)
        m_sliderMaxValue[i] = defaultMaxSliderVal;
    for (int i = 0; i < numButtons; ++i)
        m_buttonMaxValue[i] = defaultMaxButtonVal;

    SetPad_Layout(plDuet);
    m_padMode = pvmMonoContinuous;
    m_padMonoCCNumber = 11;

    SetSlider(0, 0, 1, smContinuous);
    SetSlider(1, 0, 2, smContinuous);

    SetButton(0, 1, 102, bmNote, Manta::Red, Manta::Off, Manta::Off);
    SetButton(1, 1, 103, bmNote, Manta::Red, Manta::Off, Manta::Off);
    SetButton(2, 1, 104, bmNote, Manta::Red, Manta::Off, Manta::Off);
    SetButton(3, 1, 105, bmNote, Manta::Red, Manta::Off, Manta::Off);
}

void MantaMidiSettings::SetPad_Layout(PadLayout layout)
{
    m_padLayout = layout;

    for (int i = 0; i < numPads; ++i)
        SetPadLED(i, 1, i, i+50);

    switch(layout)
    {
    case plPiano:
        AssignPianoLayout();
        SetAllPadInactiveColor(Manta::Off);
        SetAllPadOnColor(Manta::Red);
        SetAllPadOffColor(Manta::Amber);
        break;
    case plChromatic:
        AssignChromaticLayout();
        SetAllPadInactiveColor(Manta::Off);
        SetAllPadOnColor(Manta::Amber);
        SetAllPadOffColor(Manta::Off);
        break;
    case plDuet:
        AssignHaydenDuetLayout();
        SetAllPadInactiveColor(Manta::Off);
        SetAllPadOnColor(Manta::Red);
        SetAllPadOffColor(Manta::Off);
        break;
    case plHoneycomb:
    default:
        AssignHoneycombLayout();
        SetAllPadInactiveColor(Manta::Off);
        SetAllPadOnColor(Manta::Red);
        SetAllPadOffColor(Manta::Off);
    }
}

void MantaMidiSettings::AssignPianoLayout()
{
    int baseMidi = 36;
    int padIndex = 0;
    int channel = 0;
    for (int i = 0; i < (MANTA_PAD_ROWS / 2); ++i)
    {
      SetPad(padIndex, channel, baseMidi++);     // 0
      SetPad(padIndex + 8, channel, baseMidi++); // 1
      padIndex++;
      SetPad(padIndex, channel, baseMidi++);     // 2
      SetPad(padIndex + 8, channel, baseMidi++); // 3
      padIndex++;
      SetPad(padIndex, channel, baseMidi++);     // 4
      SetPad(padIndex + 8, channel, -1);
      padIndex++;
      SetPad(padIndex, channel, baseMidi++);     // 5
      SetPad(padIndex + 8, channel, baseMidi++); // 6
      padIndex++;
      SetPad(padIndex, channel, baseMidi++);     // 7
      SetPad(padIndex + 8, channel, baseMidi++); // 8
      padIndex++;
      SetPad(padIndex, channel, baseMidi++);     // 9
      SetPad(padIndex + 8, channel, baseMidi++); // 10
      padIndex++;
      SetPad(padIndex, channel, baseMidi++);     // 11
      SetPad(padIndex + 8, channel, -1);
      padIndex++;
      SetPad(padIndex, channel, baseMidi);       //12
      SetPad(padIndex + 8, channel, -1);
      padIndex += 9;
    }
}

void MantaMidiSettings::AssignChromaticLayout()
{
    int channel = 0;
    for (int i = 0; i < numPads; ++i)
        SetPad(i, channel, 36+i);
}

void MantaMidiSettings::AssignHoneycombLayout()
{
    int channel = 0;
    // First Row
    SetPad(0, channel, 32); // G#1
    SetPad(1, channel, 35); // B1
    SetPad(2, channel, 38); // D2
    SetPad(3, channel, 53); // F3
    SetPad(4, channel, 56); // G#3
    SetPad(5, channel, 59); // B3
    SetPad(6, channel, 62); // D4
    SetPad(7, channel, 77); // F5

    // Second Row
    SetPad(8, channel, 40); // E2
    SetPad(9, channel, 43); // G2
    SetPad(10, channel, 46); // A#2
    SetPad(11, channel, 61); // C#4
    SetPad(12, channel, 64); // E4
    SetPad(13, channel, 67); // G4
    SetPad(14, channel, 70); // A#4
    SetPad(15, channel, 85); // C#6

    // Third Row
    SetPad(16, channel, 24); // A1
    SetPad(17, channel, 36); // C2
    SetPad(18, channel, 39); // D#2
    SetPad(19, channel, 54); // F#3
    SetPad(20, channel, 57); // A3
    SetPad(21, channel, 60); // C4
    SetPad(22, channel, 75); // D#5
    SetPad(23, channel, 78); // F#5

    // Fourth Row
    SetPad(24, channel, 41); // F2
    SetPad(25, channel, 44); // G#2
    SetPad(26, channel, 47); // B2
    SetPad(27, channel, 62); // D4
    SetPad(28, channel, 65); // F4
    SetPad(29, channel, 68); // G#4
    SetPad(30, channel, 71); // B4
    SetPad(31, channel, 86); // D6

    // Fifth Row
    SetPad(32, channel, 34); // A#1
    SetPad(33, channel, 37); // C#2
    SetPad(34, channel, 52); // E3
    SetPad(35, channel, 55); // G3
    SetPad(36, channel, 58); // A#3
    SetPad(37, channel, 73); // C#5
    SetPad(38, channel, 76); // E5
    SetPad(39, channel, 79); // G5

    // Sixth Row
    SetPad(40, channel, 42); // F#2
    SetPad(41, channel, 45); // A2
    SetPad(42, channel, 48); // C3
    SetPad(43, channel, 63); // D#4
    SetPad(44, channel, 66); // F4
    SetPad(45, channel, 69); // A4
    SetPad(46, channel, 72); // C5
    SetPad(47, channel, 87); // D#6
}

void MantaMidiSettings::AssignHaydenDuetLayout()
{
    // First Row
    int channel = 0;
    SetPad(0, channel, 61); // Db4
    SetPad(1, channel, 63); //
    SetPad(2, channel, 65); //
    SetPad(3, channel, 67); //
    SetPad(4, channel, 69); //
    SetPad(5, channel, 71); //
    SetPad(6, channel, 73); //
    SetPad(7, channel, 75); //

    // Second Row
    SetPad(8, channel, 68); // Ab4
    SetPad(9, channel, 70); //
    SetPad(10, channel, 72); //
    SetPad(11, channel, 74); //
    SetPad(12, channel, 76); //
    SetPad(13, channel, 78); //
    SetPad(14, channel, 80); //
    SetPad(15, channel, 82); //

    // Third Row
    SetPad(16, channel, 73); // Db5
    SetPad(17, channel, 75); //
    SetPad(18, channel, 77); //
    SetPad(19, channel, 79); //
    SetPad(20, channel, 81); //
    SetPad(21, channel, 83); //
    SetPad(22, channel, 85); //
    SetPad(23, channel, 87); //

    // Fourth Row
    SetPad(24, channel, 80); // Ab5
    SetPad(25, channel, 82); //
    SetPad(26, channel, 84); //
    SetPad(27, channel, 86); //
    SetPad(28, channel, 88); //
    SetPad(29, channel, 90); //
    SetPad(30, channel, 92); //
    SetPad(31, channel, 94);

    // Fifth Row
    SetPad(32, channel, 85); // Db6
    SetPad(33, channel, 87); //
    SetPad(34, channel, 89); //
    SetPad(35, channel, 91); //
    SetPad(36, channel, 93); //
    SetPad(37, channel, 95); //
    SetPad(38, channel, 97); //
    SetPad(39, channel, 99); //

    // Sixth Row
    SetPad(40, channel, 92); // Ab6
    SetPad(41, channel, 94); //
    SetPad(42, channel, 96); //
    SetPad(43, channel, 98); //
    SetPad(44, channel, 100); //
    SetPad(45, channel, 102); //
    SetPad(46, channel, 104); //
    SetPad(47, channel, 106); //
}

