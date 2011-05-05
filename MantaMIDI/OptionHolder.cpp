#include "OptionHolder.h"

OptionHolder::OptionHolder()
{
    Reset();
}

OptionHolder::OptionHolder(int argc, char **argv)
{
    Reset();

    for (int i = 0; i < argc; ++i)
    {
     if ( 0 == strcmp(argv[i], "-d") )
       m_bDebugMode = true;
     else if ( 0 == strcmp(argv[i], "-v" ) )
       m_bUseVelocity = true;
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
                 m_padMode = pvmPolyAftertouch;
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

    PrintOptionStatus();
}

OptionHolder::~OptionHolder()
{
}

/* Master settings */
bool OptionHolder::GetDebugMode()  { return m_bDebugMode; }
bool OptionHolder::GetUseVelocity() { return m_bUseVelocity; }
void OptionHolder::SetUseVelocity(bool bUseVelocity) { m_bUseVelocity = bUseVelocity; }

/* Pads */
unsigned char OptionHolder::GetPad_EventChannel() {return m_padEventChannel; }
unsigned char OptionHolder::GetPad_Midi() { return m_basePadMidi; }
unsigned char OptionHolder::GetPad_LEDChannel() { return m_padLEDChannel; }
unsigned char OptionHolder::GetPad_AmberLEDMidi() { return m_baseAmberLEDMidi; }
unsigned char OptionHolder::GetPad_RedLEDMidi() { return m_baseRedLEDMidi; }
Manta::LEDState OptionHolder::GetPad_InactiveColor() { return m_inactivePadColor; }
Manta::LEDState OptionHolder::GetPad_OnColor() { return m_onPadColor; }
Manta::LEDState OptionHolder::GetPad_OffColor() { return m_offPadColor; }
PadLayout OptionHolder::GetPad_Layout() { return m_padLayout; }
PadValMode OptionHolder::GetPad_Mode() { return m_padMode; }

/* Sliders */
unsigned char OptionHolder::GetSlider_EventChannel(int slider) { return m_slider_EventChannel[slider]; }
unsigned char OptionHolder::GetSlider_MidiNote(int slider) { return m_slider_MidiNote[slider]; }
SliderMode OptionHolder::GetSlider_Mode(int slider) { return m_slider_Mode[slider]; }

/* Buttons */
unsigned char OptionHolder::GetButton_EventChannel(int button) { return m_buttonEventChannel[button]; }
unsigned char OptionHolder::GetBaseButtonMidi(int button) { return m_baseButtonMidi[button]; }
ButtonMode OptionHolder::GetButtonMode(int button) { return m_buttonMode[button]; }
Manta::LEDState OptionHolder::GetInactiveButtonColor(int button) { return m_inactiveButtonColor[button]; }
Manta::LEDState OptionHolder::GetOnButtonColor(int button) { return m_onButtonColor[button]; }
Manta::LEDState OptionHolder::GetOffButtonColor(int button)
{ return m_offButtonColor[button]; }
void OptionHolder::SetButton(int button, unsigned char channel, unsigned char key, ButtonMode mode, Manta::LEDState onColor, Manta::LEDState offColor, Manta::LEDState inactiveColor)
{
}

void OptionHolder::PrintOptionStatus()
{
    printf("Debug Mode: %d\n", (int)m_bDebugMode);
    printf("Velocity: %d\n\n", (int)m_bUseVelocity);

    printf("Pad Event Channel: %d\n", m_padEventChannel);
    printf("Base Pad MIDI: %d\n", m_basePadMidi);
    printf("Pad LED Channel: %d\n", m_padLEDChannel);
    printf("Base Amber LED MIDI: %d\n", m_baseAmberLEDMidi);
    printf("Base Red LED MIDI: %d\n", m_baseRedLEDMidi);
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
    printf("Button %d Mode: %d\n\n", 4, m_buttonMode[3]);
}

void OptionHolder::Reset()
{
    m_bDebugMode = false;
    m_bUseVelocity = false;

    m_padEventChannel = 0;
    m_basePadMidi = 36;
    m_padLEDChannel = 1;
    m_baseAmberLEDMidi = 0;
    m_baseRedLEDMidi = 50;
    SetPadLayout(plHoneycomb);
    m_padMode = pvmMonoAftertouch;

    m_slider0_EventChannel = 1;
    m_slider1_EventChannel = 1;
    m_slider0_MidiNote = 20;
    m_slider1_MidiNote = 21;
    m_slider0_Mode = smContinuous;
    m_slider1_Mode = smContinuous;

    m_buttonEventChannel = 2;
    m_baseButtonMidi = 102;
    m_buttonMode = bmDefault;
    m_activeButtonColor = Manta::Amber;
    m_onButtonColor = Manta::Red;
    m_offButtonColor = Manta::Off;
}

void OptionHolder::SetPadLayout(PadLayout layout)
{
    m_padLayout = layout;
    switch(layout)
    {
    case plPiano:
        m_activePadColor = Manta::Amber;
        m_inactivePadColor = Manta::Off;
        m_onPadColor = Manta::Red;
        m_offPadColor = Manta::Amber;
    break;
    case plChromatic:
        m_activePadColor = Manta::Off;
        m_inactivePadColor = Manta::Off;
        m_onPadColor = Manta::Amber;
        m_offPadColor = Manta::Off;
    break;
    case plDuet:
    case plHoneycomb:
    default:
        m_activePadColor = Manta::Off;
        m_inactivePadColor = Manta::Off;
        m_onPadColor = Manta::Red;
        m_offPadColor = Manta::Off;
    }
}
