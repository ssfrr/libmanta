#ifndef _OPTIONHOLDER_H
#define _OPTIONHOLDER_H

#include <stdio.h>
#include <string.h>

enum PadLayout
  {
    plHoneycomb = 0,
    plPiano,
    plChromatic
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
    bmDefault = 0
  };

class OptionHolder
{
 public:
 OptionHolder(int argc, char **argv) 
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
		  case 'P':
		    m_padLayout = plPiano;
		    break;
		  case 'C':
		    m_padLayout = plChromatic;
		    break;
		  default:
		    m_padLayout = plHoneycomb;
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
		    m_padMode = pvmMonoAftertouch;
		    break;
		  case '2':
		    m_padMode = pvmPolyAftertouch;
		    break;
		  default:
		    m_padMode = pvmMonoAftertouch;
		    break;
		  }
	      }
	  }
      }
    
    PrintOptionStatus();
  }
  
  ~OptionHolder() {}
  
  /* Master settings */
  bool GetDebugMode()  { return m_bDebugMode; }
  bool GetUseVelocity() { return m_bUseVelocity; }

  /* Pads */
  unsigned char GetPadEventChannel() {return m_padEventChannel; }
  unsigned char GetBasePadMidi() { return m_basePadMidi; }
  unsigned char GetPadLEDChannel() { return m_padLEDChannel; }
  unsigned char GetBaseAmberLEDMidi() { return m_baseAmberLEDMidi; }
  unsigned char GetBaseRedLEDMidi() { return m_baseRedLEDMidi; }
  PadLayout GetPadLayout() { return m_padLayout; }
  PadValMode GetPadMode() { return m_padMode; }

  /* Sliders */
  unsigned char GetSlider0_EventChannel() { return m_slider0_EventChannel; }
  unsigned char GetSlider1_EventChannel() { return m_slider1_EventChannel; }
  unsigned char GetSlider0_MidiNote() { return m_slider0_MidiNote; }
  unsigned char GetSlider1_MidiNote() { return m_slider1_MidiNote; }
  SliderMode GetSlider0_Mode() { return m_slider0_Mode; }
  SliderMode GetSlider1_Mode() { return m_slider1_Mode; }

  /* Buttons */
  unsigned char GetButtonEventChannel() { return m_buttonEventChannel; }
  unsigned char GetBaseButtonMidi() { return m_baseButtonMidi; }
  ButtonMode GetButtonMode() { return m_buttonMode; }

  void PrintOptionStatus()
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

    printf("Button Event Channel: %d\n", m_buttonEventChannel);
    printf("Button Mode: %d\n\n", m_buttonMode);
  }

  void Reset()
  {
    m_bDebugMode = false;
    m_bUseVelocity = false;

    m_padEventChannel = 1;
    m_basePadMidi = 36;
    m_padLEDChannel = 1;
    m_baseAmberLEDMidi = 0;
    m_baseRedLEDMidi = 50;
    m_padLayout = plHoneycomb;
    m_padMode = pvmMonoAftertouch;

    m_slider0_EventChannel = 2;
    m_slider1_EventChannel = 2;
    m_slider0_MidiNote = 20;
    m_slider1_MidiNote = 21;
    m_slider0_Mode = smContinuous;
    m_slider1_Mode = smContinuous;

    m_buttonEventChannel = 3;
    m_baseButtonMidi = 102;
    m_buttonMode = bmDefault;
  }
  
 private:
  /* Master Program Settings */
  bool m_bDebugMode;  
  bool m_bUseVelocity;

  /* Pad */
  unsigned char m_padEventChannel;
  unsigned char m_basePadMidi;

  unsigned char m_padLEDChannel;
  unsigned char m_baseAmberLEDMidi;
  unsigned char m_baseRedLEDMidi;

  PadLayout m_padLayout;
  PadValMode m_padMode;

  /* Slider0 */
  unsigned char m_slider0_EventChannel;
  unsigned char m_slider0_MidiNote;
  SliderMode m_slider0_Mode;

  /* Slider1 */
  unsigned char m_slider1_EventChannel;
  unsigned char m_slider1_MidiNote;
  SliderMode m_slider1_Mode;

  /* button */
  unsigned char m_buttonEventChannel;
  unsigned char m_baseButtonMidi;
  ButtonMode m_buttonMode;
  
};

#endif
