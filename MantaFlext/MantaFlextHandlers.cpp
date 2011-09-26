#include "MantaFlext.h"
#include "../core/MantaExceptions.h"

LEDState manta::ledStateFromSymbol(const t_symbol *stateSymbol)
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

LEDState manta::ledStateFromInt(int state)
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
   connectionMutex.Lock();
   if(threadRunning)
   {
      connectionMutex.Unlock();
      post("manta: Already Connected");
      return;
   }
   threadRunning = true;
   try
   {
      while(!shouldStop)
      {
         Attach();
         post("manta: manta attached");
         if(1 == ConnectedManta->GetReferenceCount())
         {
            ConnectedManta->Connect();
         }
         connectionMutex.Unlock();
         post("manta: Connected to Manta %d", ConnectedManta->GetSerialNumber());
         ConnectedManta->ResendLEDState();
         while(!shouldStop)
         {
            /* ensure that only one thread is handling events at a time. This
             * is probably excessive, but much simpler than finer-grained locking */
            connectionMutex.Lock();
            /* TODO: only one of the connected objects needs to have an active
             * thread polling the device */
            ConnectedManta->HandleEvents();
            connectionMutex.Unlock();
         }
         Detach();
      }
   }
   catch(MantaNotFoundException e)
   {
      connectionMutex.Unlock();
      post("manta: No attached Mantas found. Plug in a Manta and send \"connect\"");
   }
   catch(MantaOpenException e)
   {
      connectionMutex.Unlock();
      post("manta: Could not connect to attached Manta");
   }
   catch(MantaCommunicationException e)
   {
      connectionMutex.Unlock();
      post("manta: Communication with Manta interrupted");
   }
   threadRunning = false;
   Lock();
   cond.Signal();
   Unlock();
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
      ConnectedManta->SetLEDControl(PadAndButton, state);
   }
   else if(control == sliderSymbol)
   {
      ConnectedManta->SetLEDControl(Slider, state);
   }
   else if(control == buttonSymbol)
   {
      if(0 == state)
      {
         ConnectedManta->ClearButtonLEDs();
      }
      ConnectedManta->SetLEDControl(Button, state);
   }
}

void manta::SetPadLED(int argc, t_atom *argv)
{
   if(! Attached())
   {
      return;
   }
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
         ConnectedManta->SetPadLED(parsedState, GetInt(argv[i]));
      }
   }
}

void manta::SetPadLEDRow(t_symbol *state, int row, int mask)
{
   if(! Attached())
   {
      return;
   }
   LEDState parsedState = ledStateFromSymbol(state);
   ConnectedManta->SetPadLEDRow(parsedState, row, mask);
}

void manta::SetPadLEDRowNum(int state, int row, int mask)
{
   if(! Attached())
   {
      return;
   }
   LEDState parsedState = ledStateFromInt(state);
   ConnectedManta->SetPadLEDRow(parsedState, row, mask);
}

void manta::SetPadLEDColumn(t_symbol *state, int column, int mask)
{
   if(! Attached())
   {
      return;
   }
   LEDState parsedState = ledStateFromSymbol(state);
   ConnectedManta->SetPadLEDColumn(parsedState, column, mask);
}

void manta::SetPadLEDColumnNum(int state, int column, int mask)
{
   if(! Attached())
   {
      return;
   }
   LEDState parsedState = ledStateFromInt(state);
   ConnectedManta->SetPadLEDColumn(parsedState, column, mask);
}

void manta::SetSliderLEDMask(t_symbol *state, int id, int mask)
{
   if(! Attached())
   {
      return;
   }
   LEDState parsedState = ledStateFromSymbol(state);
   ConnectedManta->SetSliderLED(parsedState, id, mask);
}

void manta::SetSliderLEDMaskNum(int state, int id, int mask)
{
   if(! Attached())
   {
      return;
   }
   LEDState parsedState = ledStateFromInt(state);
   ConnectedManta->SetSliderLED(parsedState, id, mask);
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
      ConnectedManta->SetSliderLED(Off, id, 0xFF);
   }
}

void manta::SetSliderLEDNum(int id, int ledNum)
{
   if(! Attached())
   {
      return;
   }
   if(ledNum >= 0 && ledNum < 8)
   {
      uint8_t mask = 0x80 >> ledNum;
      ConnectedManta->SetSliderLED(Amber, id, mask);
      ConnectedManta->SetSliderLED(Off, id, ~mask);
   }
   else if(ledNum == -1)
   {
      ConnectedManta->SetSliderLED(Off, id, 0xFF);
   }
}

void manta::SetButtonLED(int argc, t_atom *argv)
{
   if(! Attached())
   {
      return;
   }
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
         ConnectedManta->SetButtonLED(parsedState, GetInt(argv[i]));
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

void manta::ClearPadAndButtonLEDs()
{
   if(! Attached())
   {
      return;
   }
   ConnectedManta->ClearPadAndButtonLEDs();
}
