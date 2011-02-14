#include "MantaFlext.h"

void manta::PadEvent(int id, int value)
{
   t_atom args[2];
   SetFloat(args[0], id + 1);
   SetFloat(args[1], value);
   ToOutAnything(continuousOutlet, padSymbol, sizeof(args) / sizeof(t_atom), args);
}

void manta::ButtonEvent(int id, int value)
{
   t_atom args[2];
   SetFloat(args[0], id + 49);
   SetFloat(args[1], value);
   ToOutAnything(continuousOutlet, buttonSymbol, sizeof(args) / sizeof(t_atom), args);
}

void manta::SliderEvent(int id, int value)
{
   t_atom args[3];
   SetFloat(args[0], id + 1);
   SetFloat(args[1], (value != 0xFFFF) ? value : lastSliderValue[id]);
   SetFloat(args[2], (value != 0xFFFF) ? 1 : 0);
   ToOutList(sliderOutlet, sizeof(args) / sizeof(t_atom), args);
   lastSliderValue[id] = value;
}

void manta::PadVelocityEvent(int id, int value)
{
   t_atom args[2];
   SetFloat(args[0], id + 1);
   SetFloat(args[1], value);
   ToOutAnything(velocityOutlet, padSymbol, sizeof(args) / sizeof(t_atom), args);
}

void manta::ButtonVelocityEvent(int id, int value)
{
   t_atom args[2];
   SetFloat(args[0], id + 49);
   SetFloat(args[1], value);
   ToOutAnything(velocityOutlet, buttonSymbol, sizeof(args) / sizeof(t_atom), args);
}

void manta::MaximumEvent(int id, int value)
{
   t_atom args[2];
   SetFloat(args[0], id);
   SetFloat(args[1], value);
   ToOutAnything(processedOutlet, maxSymbol, sizeof(args) / sizeof(t_atom), args);
}
