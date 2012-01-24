#include "MantaFlext.h"
#include <cstdarg>
#include <cstdio>

void manta::PadEvent(int row, int column, int id, int value)
{
   t_atom args[2];
   SetFloat(args[0], id + (OneIndexed ? 1 : 0));
   SetFloat(args[1], value);
   ToOutAnything(continuousOutlet, padSymbol, sizeof(args) / sizeof(t_atom), args);
}

void manta::ButtonEvent(int id, int value)
{
   t_atom args[2];
   SetFloat(args[0], id + (OneIndexed ? 1 : 0));
   SetFloat(args[1], value);
   ToOutAnything(continuousOutlet, buttonSymbol, sizeof(args) / sizeof(t_atom), args);
}

void manta::SliderEvent(int id, int value)
{
   t_atom args[3];
   SetFloat(args[0], id + (OneIndexed ? 1 : 0));
   SetFloat(args[1], (value != 0xFFFF) ? value : lastSliderValue[id]);
   SetFloat(args[2], (value != 0xFFFF) ? 1 : 0);
   ToOutList(sliderOutlet, sizeof(args) / sizeof(t_atom), args);
   lastSliderValue[id] = value;
}

void manta::PadVelocityEvent(int row, int column, int id, int value)
{
   t_atom args[2];
   SetFloat(args[0], id + (OneIndexed ? 1 : 0));
   SetFloat(args[1], value);
   ToOutAnything(velocityOutlet, padSymbol, sizeof(args) / sizeof(t_atom), args);
}

void manta::ButtonVelocityEvent(int id, int value)
{
   t_atom args[2];
   SetFloat(args[0], id + (OneIndexed ? 1 : 0));
   SetFloat(args[1], value);
   ToOutAnything(velocityOutlet, buttonSymbol, sizeof(args) / sizeof(t_atom), args);
}

void manta::FrameEvent(uint8_t *frame)
{
   /* TODO: the frame size and all the offset garbage shouldn't be here, and should
    * be pushed down into the parent classes */
   t_atom padValues[48];
   for(unsigned int i = 0; i < (sizeof(padValues) / sizeof(padValues[0])); ++i)
   {
      SetFloat(padValues[i], frame[i+1]);
   }
   ToOutList(frameOutlet, sizeof(padValues) / sizeof(padValues[0]),
         padValues);
}

/*
void manta::DebugPrint(const char *fmt, ...)
{
   if(DebugEnabled)
   {
      va_list args;
      char string[256];
      va_start(args, fmt);
      vsprintf(string, fmt, args);
      va_end (args);
      post(string);
   }
}
*/
