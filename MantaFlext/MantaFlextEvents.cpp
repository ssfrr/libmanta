#include "MantaFlext.h"

void manta::PadEvent(int row, int column, int id, int value)
{
   t_atom args[2];
   SetFloat(args[0], id);
   SetFloat(args[1], value);
   ToOutAnything(continuousOutlet, padSymbol, sizeof(args) / sizeof(t_atom), args);
}

void manta::ButtonEvent(int id, int value)
{
   t_atom args[2];
   SetFloat(args[0], id);
   SetFloat(args[1], value);
   ToOutAnything(continuousOutlet, buttonSymbol, sizeof(args) / sizeof(t_atom), args);
}

void manta::SliderEvent(int id, int value)
{
   t_atom args[3];
   SetFloat(args[0], id);
   SetFloat(args[1], (value != 0xFFFF) ? value : lastSliderValue[id]);
   SetFloat(args[2], (value != 0xFFFF) ? 1 : 0);
   ToOutList(sliderOutlet, sizeof(args) / sizeof(t_atom), args);
   lastSliderValue[id] = value;
}

void manta::PadVelocityEvent(int row, int column, int id, int value)
{
   t_atom args[2];
   SetFloat(args[0], id);
   SetFloat(args[1], value);
   ToOutAnything(velocityOutlet, padSymbol, sizeof(args) / sizeof(t_atom), args);
}

void manta::ButtonVelocityEvent(int id, int value)
{
   t_atom args[2];
   SetFloat(args[0], id);
   SetFloat(args[1], value);
   ToOutAnything(velocityOutlet, buttonSymbol, sizeof(args) / sizeof(t_atom), args);
}

void manta::FrameReceived(int8_t *frame)
{
   t_atom padValues[48];
   for(unsigned int i = 0; i < (sizeof(padValues) / sizeof(padValues[0])); ++i)
   {
      SetFloat(padValues[i], frame[i+1] + 128);
   }
   ToOutList(frameOutlet, sizeof(padValues) / sizeof(padValues[0]),
         padValues);
   
   /* call the parent class version, to make sure all the other handlers get
    * called properly */
   Manta::FrameReceived(frame);
}
