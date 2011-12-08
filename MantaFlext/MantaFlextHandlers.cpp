#include "MantaFlext.h"
#include "../core/MantaExceptions.h"

MantaServer::LEDState manta::ledStateFromSymbol(const t_symbol *stateSymbol)
{
   if(stateSymbol == amberSymbol)
   {
      return MantaServer::Amber;
   }
   else if(stateSymbol == redSymbol)
   {
       if (ConnectedManta->GetSerialNumber() >= 70)
       { 
           return MantaServer::Red;
       }
       else
       {
           return MantaServer::Amber;
       }
   }
   else
   {
      return MantaServer::Off;
   }
}

MantaServer::LEDState manta::ledStateFromInt(int state)
{
   if(state == 1)
   {
      return MantaServer::Amber;
   }
   else if(state == 2)
   {
       if (ConnectedManta->GetSerialNumber() >= 70)
       { 
           return MantaServer::Red;
       }
       else
       {
           return MantaServer::Amber;
       }
   }
   else
   {
      return MantaServer::Off;
   }
}

void manta::SetLEDControl(t_symbol *control, int state)
{
   if(! Attached())
   {
      return;
   }
   if(control == padAndButtonSymbol)
   {
      if(0 == state)
      {
         ClearPadAndButtonLEDs();
      }
      ConnectedManta->SetLEDControl(MantaServer::PadAndButton, state);
   }
   else if(control == sliderSymbol)
   {
      ConnectedManta->SetLEDControl(MantaServer::Slider, state);
   }
   else if(control == buttonSymbol)
   {
      if(0 == state)
      {
         ConnectedManta->ClearButtonLEDs();
      }
      ConnectedManta->SetLEDControl(MantaServer::Button, state);
   }
}

void manta::SetPadLED(int argc, t_atom *argv)
{
   if(! Attached())
   {
      return;
   }
   MantaServer::LEDState parsedState;
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
         ConnectedManta->SetPadLED(parsedState, GetInt(argv[i]) - (OneIndexed ? 1 : 0));
      }
   }
}

void manta::SetPadLEDRow(t_symbol *state, int row, int mask)
{
   if(! Attached())
   {
      return;
   }
   MantaServer::LEDState parsedState = ledStateFromSymbol(state);
   ConnectedManta->SetPadLEDRow(parsedState, row - (OneIndexed ? 1 : 0), mask);
}

void manta::SetPadLEDRowNum(int state, int row, int mask)
{
   if(! Attached())
   {
      return;
   }
   MantaServer::LEDState parsedState = ledStateFromInt(state);
   ConnectedManta->SetPadLEDRow(parsedState, row - (OneIndexed ? 1 : 0), mask);
}

void manta::SetPadLEDColumn(t_symbol *state, int column, int mask)
{
   if(! Attached())
   {
      return;
   }
   MantaServer::LEDState parsedState = ledStateFromSymbol(state);
   ConnectedManta->SetPadLEDColumn(parsedState, column - (OneIndexed ? 1 : 0), mask);
}

void manta::SetPadLEDColumnNum(int state, int column, int mask)
{
   if(! Attached())
   {
      return;
   }
   MantaServer::LEDState parsedState = ledStateFromInt(state);
   ConnectedManta->SetPadLEDColumn(parsedState, column - (OneIndexed ? 1 : 0), mask);
}

void manta::SetSliderLEDMask(t_symbol *state, int id, int mask)
{
   if(! Attached())
   {
      return;
   }
   MantaServer::LEDState parsedState = ledStateFromSymbol(state);
   ConnectedManta->SetSliderLED(parsedState, id - (OneIndexed ? 1 : 0), mask);
}

void manta::SetSliderLEDMaskNum(int state, int id, int mask)
{
   if(! Attached())
   {
      return;
   }
   MantaServer::LEDState parsedState = ledStateFromInt(state);
   ConnectedManta->SetSliderLED(parsedState, id - (OneIndexed ? 1 : 0), mask);
}

void manta::SetSliderLED(int id, t_symbol *state)
{
   if(! Attached())
   {
      return;
   }
   if(state == offSymbol)
   {
      /* turn all LEDs on that slider off */
      ConnectedManta->SetSliderLED(MantaServer::Off, id, 0xFF);
   }
}

void manta::SetSliderLEDNum(int id, int ledNum)
{
   if(! Attached())
   {
      return;
   }
   if(OneIndexed)
   {
      id -= 1;
      ledNum -= 1;
   }
   if(ledNum >= 0 && ledNum < 8)
   {
      uint8_t mask = 0x80 >> ledNum;
      ConnectedManta->SetSliderLED(MantaServer::Amber, id, mask);
      ConnectedManta->SetSliderLED(MantaServer::Off, id, ~mask);
   }
   else if(ledNum == -1)
   {
      ConnectedManta->SetSliderLED(MantaServer::Off, id, 0xFF);
   }
}

void manta::SetButtonLED(int argc, t_atom *argv)
{
   if(! Attached())
   {
      return;
   }
   MantaServer::LEDState parsedState;
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
         ConnectedManta->SetButtonLED(parsedState, GetInt(argv[i]) - (OneIndexed ? 1 : 0));
      }
   }
}

void manta::Recalibrate()
{
   if(! Attached())
   {
      return;
   }
   ConnectedManta->Recalibrate();
}

void manta::Connect(int argc, t_atom *argv)
{
   if(argc > 0 && CanbeInt(argv[0]))
   {
      Attach(GetInt(argv[0]));
   }
   else
   {
      Attach();
   }
}

void manta::EnableDebug(int enabled)
{
   DebugEnabled = enabled;
}

void manta::ClearPadAndButtonLEDs()
{
   if(! Attached())
   {
      return;
   }
   ConnectedManta->ClearPadAndButtonLEDs();
}

void manta::SetTurboMode(int state)
{
   if(! Attached())
   {
      return;
   }
   ConnectedManta->SetTurboMode(state);
}

void manta::SetRawMode(int state)
{
   if(! Attached())
   {
      return;
   }
   ConnectedManta->SetRawMode(state);
}

void manta::SetOneIndexed(int enabled)
{
   OneIndexed = enabled;
}

