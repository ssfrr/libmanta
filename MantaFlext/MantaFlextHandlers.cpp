#include "MantaFlext.h"
#include "../core/MantaExceptions.h"

manta::LEDState manta::ledStateFromSymbol(t_symbol *stateSymbol)
{
   if(stateSymbol == amberSymbol)
   {
      return Amber;
   }
   else if(stateSymbol == redSymbol)
   {
      return Red;
   }
   else
   {
      return Off;
   }
}

manta::LEDState manta::ledStateFromInt(int state)
{
   if(state == 1)
   {
      return Amber;
   }
   else if(state == 2)
   {
      return Red;
   }
   else
   {
      return Off;
   }
}

void manta::StartThread()
{
   while(!shouldStop)
   {
      try
      {
         Connect();
         post("manta: Connected to Manta");
         while(!shouldStop)
         {
            HandleEvents();
         }
      }
      catch(MantaNotFoundException e)
      {
         post("manta: No attached Mantas found. Retrying...");
         Sleep(1);
      }
      catch(MantaOpenException e)
      {
         post("manta: Could not connect to attached Manta");
         shouldStop = true;
      }
      catch(MantaCommunicationException e)
      {
         post("manta: Communication with Manta interrupted. Reconnecting...");
         Sleep(1);
      }
   }
   Lock();
   cond.Signal();
   Unlock();
}

void manta::SetLEDControl(t_symbol *control, int state)
{
   if(control == padAndButtonSymbol)
   {
      Manta::SetLEDControl(PadAndButton, state);
   }
   else if(control == sliderSymbol)
   {
      Manta::SetLEDControl(Slider, state);
   }
   else if(control == buttonSymbol)
   {
      Manta::SetLEDControl(Button, state);
   }
}

void manta::SetPadLED(t_symbol *state, int ledID)
{
   LEDState parsedState = ledStateFromSymbol(state);
   Manta::SetPadLED(parsedState, ledID);
}

void manta::SetPadLEDNum(int state, int ledID)
{
   LEDState parsedState = ledStateFromInt(state);
   Manta::SetPadLED(parsedState, ledID);
}

void manta::SetPadLEDRow(t_symbol *state, int row, int mask)
{
   LEDState parsedState = ledStateFromSymbol(state);
   Manta::SetPadLEDRow(parsedState, row, mask);
}

void manta::SetPadLEDRowNum(int state, int row, int mask)
{
   LEDState parsedState = ledStateFromInt(state);
   Manta::SetPadLEDRow(parsedState, row, mask);
}

void manta::SetPadLEDColumn(t_symbol *state, int column, int mask)
{
   LEDState parsedState = ledStateFromSymbol(state);
   Manta::SetPadLEDColumn(parsedState, column, mask);
}

void manta::SetPadLEDColumnNum(int state, int column, int mask)
{
   LEDState parsedState = ledStateFromInt(state);
   Manta::SetPadLEDColumn(parsedState, column, mask);
}

void manta::SetSliderLEDMask(t_symbol *state, int id, int mask)
{
   LEDState parsedState = ledStateFromSymbol(state);
   Manta::SetSliderLED(parsedState, id, mask);
}

void manta::SetSliderLEDMaskNum(int state, int id, int mask)
{
   LEDState parsedState = ledStateFromInt(state);
   Manta::SetSliderLED(parsedState, id, mask);
}

void manta::SetSliderLED(int id, t_symbol *state)
{
   if(state == offSymbol)
   {
      /* turn all LEDs on that slider off */
      Manta::SetSliderLED(Off, id, 0xFF);
   }
}

void manta::SetSliderLEDNum(int id, int ledNum)
{
   if(ledNum >= 0 && ledNum < 8)
   {
      uint8_t mask = 0x80 >> ledNum;
      Manta::SetSliderLED(Amber, id, mask);
      Manta::SetSliderLED(Off, id, ~mask);
   }
   else if(ledNum == -1)
   {
      Manta::SetSliderLED(Off, id, 0xFF);
   }
}

void manta::SetButtonLED(t_symbol *state, int id)
{
   LEDState parsedState = ledStateFromSymbol(state);
   Manta::SetButtonLED(parsedState, id);
}

void manta::SetButtonLEDNum(int state, int id)
{
   LEDState parsedState = ledStateFromInt(state);
   Manta::SetButtonLED(parsedState, id);
}
