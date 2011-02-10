#include "MantaFlext.h"

void manta::StartThread()
{
   Connect();
   if(running)
   {
      return;
   }
   running = true;
   while(!ShouldExit() && !shouldStop)
   {
      HandleEvents();
   }
   running = false;
   cond.Signal();
}

/* TODO: other control types */
void manta::SetLEDControl(t_symbol *control, int state)
{
   LEDControlType parsedControl;
   if(control == padAndButtonSymbol)
   {
      parsedControl = PadAndButton;
   }
   Manta::SetLEDControl(parsedControl, state);
}

void manta::SetPadLED(t_symbol *state, int ledID)
{
   LEDState parsedState;
   if(state == amberSymbol)
   {
      parsedState = Amber;
   }
   else if(state == redSymbol)
   {
      parsedState = Red;
   }
   else if(state == offSymbol)
   {
      parsedState = Off;
   }
   Manta::SetPadLED(parsedState, ledID);
}
