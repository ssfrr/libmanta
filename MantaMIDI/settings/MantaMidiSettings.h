#ifndef _OPTIONHOLDER_H
#define _OPTIONHOLDER_H

#include "../../core/Manta.h"
#include <stdio.h>
#include <string.h>

#define MANTA_PAD_ROWS  6

enum PadLayout
  {
    plHoneycomb = 0,
    plPiano,
    plChromatic,
    plDuet
  };

enum PadValMode
  {
    pvmMonoContinuous = 0,
    pvmPolyAftertouch = 1,
    pvmPolyContinuous
  };

enum SliderMode
  {
    smContinuous = 0,
    smPitchBend = 1
  };

enum ButtonMode
  {
    bmNote = 0,
    bmController = 1
  };

class MantaMidiSettings
{
 public:
    MantaMidiSettings();
    MantaMidiSettings(int argc, char **argv);

    ~MantaMidiSettings();

    /* Arg and input handling */
    bool LoadArgs(int argc, char **argv);
    void PrintUsage();

    /* Master settings */
    bool            GetDebugMode();
    bool            GetUseVelocity();
    void            SetUseVelocity(bool bUseVelocity);

    /* Pads Sends*/
    unsigned char   GetPad_EventChannel(int pad);
    char            GetPad_Midi(int pad);
    char            GetPad_MonoCCNumber();
    unsigned char   GetPad_MaxVal(int pad);
    Manta::LEDState GetPad_InactiveColor(int pad);
    Manta::LEDState GetPad_OnColor(int pad);
    Manta::LEDState GetPad_OffColor(int pad);
    PadLayout       GetPad_Layout();
    void            SetPad_Layout(PadLayout layout);
    PadValMode      GetPad_Mode();
    void            SetPad_Mode(PadValMode mode);
    void            SetPad(int pad, unsigned char channel, unsigned char note);
    void            SetPad_MIDINote(int pad, unsigned char note);
    void            SetPad_MonoCCNumber(int ccnum);
    void            CalibratePad(int pad, unsigned char value);
    void            SetAllPadOnColor(Manta::LEDState onColor);
    void            SetAllPadOffColor(Manta::LEDState offColor);
    void            SetAllPadInactiveColor(Manta::LEDState inactiveColor);

    /* Pad Recieve */
    unsigned char   GetPad_LEDChannel(int pad);
    unsigned char   GetPad_AmberLEDMidi(int pad);
    unsigned char   GetPad_RedLEDMidi(int pad);
    int             GetPadFromMidi(unsigned char midiIn, Manta::LEDState &color);
    void            SetPadLED_MidiChannel(int pad, unsigned char channel);
    void            SetPadLED_AmberMidiNote(int pad, unsigned char amberNote);
    void            SetPadLED_RedMidiNote(int pad, unsigned char redNote);
    void            SetPadLED(int pad, unsigned char channel, unsigned char amberNote, unsigned char redNote);

    /* Sliders */
    bool IsValidSliderIndex(int slider);
    unsigned char GetSlider_EventChannel(int slider);
    void SetSlider_Channel(int slider, unsigned char channel);
    char GetSlider_MidiNote(int slider);
    void SetSlider_Midi(int slider, char midi);
    unsigned short GetSlider_MaxVal(int slider);
    SliderMode GetSlider_Mode(int slider);
    void SetSlider_Mode(int slider, SliderMode mode);
    void SetSlider(int slider, unsigned char channel, unsigned char note, SliderMode mode);
    void CalibrateSlider(int slider, int value);

    /* Buttons */
    bool IsValidButtonIndex(int button);
    ButtonMode GetButton_Mode(int button);
    void SetButton_Mode(int button, ButtonMode mode);
    unsigned char GetButton_EventChannel(int button);
    void SetButton_Channel(int button, unsigned char channel);
    char GetButton_Midi(int button);
    void SetButton_Midi(int button, char midi);
    unsigned char GetButton_MaxValue(int button);
    Manta::LEDState GetButton_OnColor(int button);
    Manta::LEDState GetButton_OffColor(int button);
    Manta::LEDState GetButton_InactiveColor(int button);
    void SetButton_OnColor(int button, Manta::LEDState color);
    void SetButton_OffColor(int button, Manta::LEDState color);
    void SetButton_InactiveColor(int button, Manta::LEDState color);
    void SetButton(int button, unsigned char channel, unsigned char key, ButtonMode mode, Manta::LEDState onColor, Manta::LEDState offColor, Manta::LEDState inactiveColor);
    void CalibrateButton(int button, int value);

    void PrintOptionStatus();
    void Reset();
  
 private:    
    void AssignPianoLayout();
    void AssignChromaticLayout();
    void AssignHoneycombLayout();
    void AssignHaydenDuetLayout();

    void LoadSettings(bool bPrint=false);

protected:
    char m_layoutPath[255];

    /* Master Program Settings */
    bool m_bDebugMode;
    bool m_bUseVelocity;

    /* Pad Send*/
    const static int numPads = 48;
    const static int defaultMaxPadVal = 210;
    PadLayout m_padLayout;
    PadValMode m_padMode;
    unsigned char m_padEventChannel[numPads];
    unsigned char m_padMonoCCNumber;
    unsigned char m_basePadMidi[numPads];
    unsigned char m_padMaxValue[numPads];
    Manta::LEDState m_inactivePadColor[numPads];
    Manta::LEDState m_onPadColor[numPads];
    Manta::LEDState m_offPadColor[numPads];
    /* Pad Receive */
    unsigned char m_padLEDChannel[numPads];
    unsigned char m_AmberLEDMidi[numPads];
    unsigned char m_RedLEDMidi[numPads];

    /* Sliders */
    const static int numSliders = 2;
    const static int defaultMaxSliderVal = 4096;
    unsigned char m_sliderEventChannel[numSliders];
    unsigned char m_sliderMidiNote[numSliders];
    unsigned short m_sliderMaxValue[numSliders];
    SliderMode m_sliderMode[numSliders];

    /* button */
    const static int numButtons = 4;
    const static int defaultMaxButtonVal = 210;
    ButtonMode m_buttonMode[numButtons];
    unsigned char m_buttonEventChannel[numButtons];
    char m_buttonMidi[numButtons];
    unsigned char m_buttonMaxValue[numButtons];
    Manta::LEDState m_inactiveButtonColor[numButtons];
    Manta::LEDState m_onButtonColor[numButtons];
    Manta::LEDState m_offButtonColor[numButtons];
  
};

#endif
