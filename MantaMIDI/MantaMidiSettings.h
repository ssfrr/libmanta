#ifndef _OPTIONHOLDER_H
#define _OPTIONHOLDER_H

#include "../core/Manta.h"
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
    pvmMonoAftertouch = 0,
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

    /* Master settings */
    bool GetDebugMode();
    bool GetUseVelocity();
    void SetUseVelocity(bool bUseVelocity);

    /* Pads */
    unsigned char GetPad_EventChannel(int pad);
    char GetPad_Midi(int pad);
    unsigned char GetPad_LEDChannel(int pad);
    unsigned char GetPad_AmberLEDMidi(int pad);
    unsigned char GetPad_RedLEDMidi(int pad);
    Manta::LEDState GetPad_InactiveColor(int pad);
    Manta::LEDState GetPad_OnColor(int pad);
    Manta::LEDState GetPad_OffColor(int pad);
    PadLayout GetPad_Layout();
    void SetPad_Layout(PadLayout layout);
    PadValMode GetPad_Mode();
    void SetPad_Mode(PadValMode mode);
    void SetPad(int pad, unsigned char channel, unsigned char note);
    void SetPadLED(int pad, unsigned char channel, unsigned char amberNote, unsigned char redNote);
    void SetAllPadOnColor(Manta::LEDState onColor);
    void SetAllPadOffColor(Manta::LEDState offColor);
    void SetAllPadInactiveColor(Manta::LEDState inactiveColor);

    /* Sliders */
    bool IsValidSliderIndex(int slider);
    unsigned char GetSlider_EventChannel(int slider);
    void SetSlider_Channel(int slider, unsigned char channel);
    char GetSlider_MidiNote(int slider);
    void SetSlider_Midi(int slider, char midi);
    SliderMode GetSlider_Mode(int slider);
    void SetSlider_Mode(int slider, SliderMode mode);
    void SetSlider(int slider, unsigned char channel, unsigned char note, SliderMode mode);

    /* Buttons */
    bool IsValidButtonIndex(int button);
    ButtonMode GetButton_Mode(int button);
    void SetButton_Mode(int button, ButtonMode mode);
    unsigned char GetButton_EventChannel(int button);
    void SetButton_Channel(int button, unsigned char channel);
    char GetButton_Midi(int button);
    void SetButton_Midi(int button, char midi);
    Manta::LEDState GetButton_OnColor(int button);
    Manta::LEDState GetButton_OffColor(int button);
    Manta::LEDState GetButton_InactiveColor(int button);
    void SetButton_OnColor(int button, Manta::LEDState color);
    void SetButton_OffColor(int button, Manta::LEDState color);
    void SetButton_InactiveColor(int button, Manta::LEDState color);
    void SetButton(int button, unsigned char channel, unsigned char key, ButtonMode mode, Manta::LEDState onColor, Manta::LEDState offColor, Manta::LEDState inactiveColor);

    void PrintOptionStatus();
    void Reset();
  
 private:    
    void AssignPianoLayout();
    void AssignChromaticLayout();
    void AssignHoneycombLayout();
    void AssignHaydenDuetLayout();

protected:
    /* Master Program Settings */
    bool m_bDebugMode;
    bool m_bUseVelocity;

    /* Pad */
    const static int numPads = 48;
    PadLayout m_padLayout;
    PadValMode m_padMode;
    unsigned char m_padEventChannel[numPads];
    unsigned char m_basePadMidi[numPads];
    unsigned char m_padLEDChannel[numPads];
    unsigned char m_AmberLEDMidi[numPads];
    unsigned char m_RedLEDMidi[numPads];
    Manta::LEDState m_inactivePadColor[numPads];
    Manta::LEDState m_onPadColor[numPads];
    Manta::LEDState m_offPadColor[numPads];

    /* Sliders */
    const static int numSliders = 2;
    unsigned char m_slider_EventChannel[numSliders];
    unsigned char m_slider_MidiNote[numSliders];
    SliderMode m_slider_Mode[numSliders];

    /* button */
    const static int numButtons = 4;
    ButtonMode m_buttonMode[numButtons];
    unsigned char m_buttonEventChannel[numButtons];
    char m_buttonMidi[numButtons];
    Manta::LEDState m_inactiveButtonColor[numButtons];
    Manta::LEDState m_onButtonColor[numButtons];
    Manta::LEDState m_offButtonColor[numButtons];
  
};

#endif
