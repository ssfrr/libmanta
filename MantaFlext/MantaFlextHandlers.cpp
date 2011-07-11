#include "MantaFlext.h"
#include "../core/MantaExceptions.h"

manta::LEDState manta::ledStateFromSymbol(const t_symbol *stateSymbol)
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
   bool errorPrinted = false;
   while(!shouldStop)
   {
      try
      {
         Connect();
         post("manta: Connected to Manta");
         errorPrinted = false;
         ResendLEDState();
         while(!shouldStop)
         {
            HandleEvents();
         }
      }
      catch(MantaNotFoundException e)
      {
         if(! errorPrinted)
         {
            post("manta: No attached Mantas found. Retrying...");
            errorPrinted = true;
         }
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
      if(0 == state)
      {
         ClearPadAndButtonLEDs();
      }
      Manta::SetLEDControl(PadAndButton, state);
   }
   else if(control == sliderSymbol)
   {
      Manta::SetLEDControl(Slider, state);
   }
   else if(control == buttonSymbol)
   {
      if(0 == state)
      {
         Manta::ClearButtonLEDs();
      }
      Manta::SetLEDControl(Button, state);
   }
}

void manta::SetPadLED(int argc, t_atom *argv)
{
   LEDState parsedState;
   if(argc < 2 ||
         (!CanbeInt(argv[0]) && !IsSymbol(argv[0])))
   {
      post("manta: 'pad' - invalid parameters");
   }
   if(CanbeInt(argv[0]))
   {
      parsedState = ledStateFromInt(GetInt(argv[0]));
   }
   else if(IsSymbol(argv[0]))
   {
      parsedState = ledStateFromSymbol(GetSymbol(argv[0]));
   }
   for(int i = 1; i < argc; ++i)
   {
      if(CanbeInt(argv[i]))
      {
         Manta::SetPadLED(parsedState, GetInt(argv[i]));
      }
   }
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

void manta::SetButtonLED(int argc, t_atom *argv)
{
   LEDState parsedState;
   if(argc < 2 ||
         (!CanbeInt(argv[0]) && !IsSymbol(argv[0])))
   {
      post("manta: 'pad' - invalid parameters");
   }
   if(CanbeInt(argv[0]))
   {
      parsedState = ledStateFromInt(GetInt(argv[0]));
   }
   else if(IsSymbol(argv[0]))
   {
      parsedState = ledStateFromSymbol(GetSymbol(argv[0]));
   }
   for(int i = 1; i < argc; ++i)
   {
      if(CanbeInt(argv[i]))
      {
         Manta::SetButtonLED(parsedState, GetInt(argv[i]));
      }
   }
}
