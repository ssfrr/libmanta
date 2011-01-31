#include <libusb-1.0/libusb.h>
#include "Manta.h"
#include "MantaExceptions.h"

Manta::Manta(void) {
   LastInReport[0] = 0;
   for(int i = 1; i < 53; ++i)
   {
      LastInReport[i] = -128;
   }
   for(int i = 53; i < 57; ++i)
   {
      LastInReport[i] = 127;
   }
   for(int i = 0; i < sizeof(CurrentOutReport); ++i)
   {
      CurrentOutReport[i] = 0;
   }
}

void Manta::FrameReceived(int8_t *frame)
{
   for(int i = 0; i < 49; ++i)
   {
      if(frame[i] != LastInReport[i])
      {
         PadEvent(i - 1, frame[i] + 128);
      }
      LastInReport[i] = frame[i];
   }
   for(int i = 49; i < 53; ++i)
   {
      if(frame[i] != LastInReport[i])
      {
         ButtonEvent(i - 49, frame[i] + 128);
      }
      LastInReport[i] = frame[i];
   }
   if(frame[53] != LastInReport[53] || frame[54] != LastInReport[54])
   {
      SliderEvent(0, (frame[53] + 128) | ((frame[54] + 128) << 8 ));
   }
   if(frame[55] != LastInReport[55] || frame[56] != LastInReport[56])
   {
      SliderEvent(1, (frame[55] + 128) | ((frame[56] + 128) << 8 ));
   }
   for(int i = 53; i < 57; ++i)
   {
      LastInReport[i] = frame[i];
   }
}

void Manta::SetPadLED(LEDState state, int ledID)
{
   int baseIndex;
   int inverseIndex;

   int row = ledID / 8;
   int column = ledID % 8;

   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   
   if(ledID < 0 || ledID > 47)
   {
      throw std::invalid_argument("Invalid Pad Index");
   }

   switch(state)
   {
      case Amber:
         CurrentOutReport[AmberIndex + row] |= (1 << column);
         CurrentOutReport[RedIndex + row] &= ~(1 << column);
         break;
      case Red:
         CurrentOutReport[RedIndex + row] |= (1 << column);
         CurrentOutReport[AmberIndex + row] &= ~(1 << column);
         break;
      case Off:
         CurrentOutReport[AmberIndex + row] &= ~(1 << column);
         CurrentOutReport[RedIndex + row] &= ~(1 << column);
         break;
      default:
         throw std::invalid_argument("Invalid state");
   }

   WriteFrame(CurrentOutReport);
}

void Manta::SetPadLEDRow(LEDState state, int row, uint8_t mask)
{
   int baseIndex;
   int inverseIndex;
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   if(row < 0 || row > 5)
   {
      throw std::invalid_argument("Invalid Row Index");
   }

   switch(state)
   {
      case Amber:
         CurrentOutReport[AmberIndex + row] |= byteReverse(mask);
         CurrentOutReport[RedIndex + row] &= ~byteReverse(mask);
         break;
      case Red:
         CurrentOutReport[RedIndex + row] |= byteReverse(mask);
         CurrentOutReport[AmberIndex + row] &= ~byteReverse(mask);
         break;
      case Off:
         CurrentOutReport[RedIndex + row] &= ~byteReverse(mask);
         CurrentOutReport[AmberIndex + row] &= ~byteReverse(mask);
         break;
      default:
         throw std::invalid_argument("Invalid state");
   }
   WriteFrame(CurrentOutReport);
}

void Manta::SetPadLEDColumn(LEDState state, int column, uint8_t mask)
{
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   if(column < 0 || column > 7)
   {
      throw std::invalid_argument("Invalid Column Index");
   }
   
   switch(state)
   {
      case Amber:
         for(int i = 0; i < 6; ++i)
         {
            if((mask >> i) & 0x01)
            {
               CurrentOutReport[AmberIndex + i] |= (0x01 << column);
               CurrentOutReport[RedIndex + i] &= ~(0x01 << column);
            }
         }
         break;
      case Red:
         for(int i = 0; i < 6; ++i)
         {
            if((mask >> i) & 0x01)
            {
               CurrentOutReport[RedIndex + i] |= (0x01 << column);
               CurrentOutReport[AmberIndex + i] &= ~(0x01 << column);
            }
         }
         break;
      case Off:
         for(int i = 0; i < 6; ++i)
         {
            if((mask >> i) & 0x01)
            {
               CurrentOutReport[RedIndex + i] &= ~(0x01 << column);
               CurrentOutReport[AmberIndex + i] &= ~(0x01 << column);
            }
         }
         break;
      default:
         throw std::invalid_argument("Invalid state");
   }

   WriteFrame(CurrentOutReport);
}

void Manta::SetPadLEDFrame(LEDState state, LEDFrame mask)
{
   int baseIndex;
   int inverseIndex;
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   
   switch(state)
   {
      case Amber:
         for(int i = 0; i < sizeof(LEDFrame); ++i)
         {
            CurrentOutReport[AmberIndex + i] = byteReverse(mask[i]);
            CurrentOutReport[RedIndex + i] &= ~byteReverse(mask[i]);
         }
         break;
      case Red:
         for(int i = 0; i < sizeof(LEDFrame); ++i)
         {
            CurrentOutReport[RedIndex + i] = byteReverse(mask[i]);
            CurrentOutReport[AmberIndex + i] &= ~byteReverse(mask[i]);
         }
         break;
      case Off:
         for(int i = 0; i < sizeof(LEDFrame); ++i)
         {
            CurrentOutReport[RedIndex + i] &= ~byteReverse(mask[i]);
            CurrentOutReport[AmberIndex + i] &= ~byteReverse(mask[i]);
         }
         break;
      default:
         throw std::invalid_argument("Invalid state");
   }
   WriteFrame(CurrentOutReport);
}

void Manta::SetSliderLED(LEDState state, int id, uint8_t mask)
{
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   if(id < 0 || id > 1)
   {
      throw std::invalid_argument("Invalid Slider Index");
   }
   switch(state)
   {
      case Amber:
         CurrentOutReport[SliderIndex + id] |= byteReverse(mask);
         break;
      case Red:
         /* no Red slider LEDs, do nothing */
         break;
      case Off:
         CurrentOutReport[SliderIndex + id] &= ~byteReverse(mask);
         break;
      default:
         throw std::invalid_argument("Invalid state");
   }
   WriteFrame(CurrentOutReport);
}

void Manta::SetButtonLED(LEDState state, int id)
{
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   if(id < 0 || id > 3)
   {
      throw std::invalid_argument("Invalid Button Index");
   }
   
   switch(state)
   {
      case Amber:
         CurrentOutReport[ButtonIndex] |= (0x01 << (id));
         CurrentOutReport[ButtonIndex] &= ~(0x01 << (id + 4));
         break;
      case Red:
         CurrentOutReport[ButtonIndex] |= (0x01 << (id + 4));
         CurrentOutReport[ButtonIndex] &= ~(0x01 << (id));
         break;
      case Off:
         CurrentOutReport[ButtonIndex] &= ~(0x01 << (id + 4));
         CurrentOutReport[ButtonIndex] &= ~(0x01 << (id));
         break;
      default:
         throw std::invalid_argument("Invalid state");
   }
   WriteFrame(CurrentOutReport);
}

void Manta::Recalibrate(void)
{
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   
   CurrentOutReport[ConfigIndex] |= 0x40;
   WriteFrame(CurrentOutReport);
   CurrentOutReport[ConfigIndex] &= ~0x40;
   WriteFrame(CurrentOutReport);
}

void Manta::SetLEDControl(LEDControlType control, bool state)
{
   uint8_t flag;
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   
   switch(control)
   {
      case PadAndButton:
         flag = 0x01;
         break;
      case Slider:
         flag = 0x02;
         break;
      case Button:
         flag = 0x20;
         break;
      default:
         throw std::invalid_argument("Invalid Control Type");
   }

   if(state)
      CurrentOutReport[ConfigIndex] |= flag;
   else
      CurrentOutReport[ConfigIndex] &= ~flag;
   WriteFrame(CurrentOutReport);
}

void Manta::SetTurboMode(bool Enabled)
{
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   
   if(Enabled)
      CurrentOutReport[ConfigIndex] |= 0x04;
   else
      CurrentOutReport[ConfigIndex] &= ~0x04;
   WriteFrame(CurrentOutReport);
}

void Manta::SetRawMode(bool Enabled)
{
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   
   if(Enabled)
      CurrentOutReport[ConfigIndex] |= 0x08;
   else
      CurrentOutReport[ConfigIndex] &= ~0x08;
   WriteFrame(CurrentOutReport);
}

void Manta::SetHiResMode(bool Enabled)
{
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   
   if(Enabled)
      CurrentOutReport[ConfigIndex] |= 0x10;
   else
      CurrentOutReport[ConfigIndex] &= ~0x10;
   WriteFrame(CurrentOutReport);
}

uint8_t Manta::byteReverse(uint8_t inByte)
{
   // Algorithm from Bit Twiddling Hacks
   uint8_t outByte = inByte; // first get LSB of inByte
   int s = 7; // extra shift needed at end

   for (inByte >>= 1; inByte; inByte >>= 1)
   {   
      outByte <<= 1;
      outByte |= inByte & 1;
      s--;
   }
   outByte <<= s; // shift when inByte's highest bits are zero
}
