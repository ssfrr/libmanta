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

void Manta::SetLED(LEDColor color, int column, int row, bool enabled)
{
   int baseIndex;
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   
   switch(color)
   {
      case Amber:
         baseIndex = 0;
         break;
      case Red:
         baseIndex = 10;
         break;
      default:
         throw std::invalid_argument("Invalid Color");
   }
   if(row >= 6)
   {
      throw std::invalid_argument("Invalid Row Index");
   }
   if(column >= 8)
   {
      throw std::invalid_argument("Invalid Column Index");
   }

   if(enabled)
      CurrentOutReport[baseIndex + row] |= (1 << row);
   else
      CurrentOutReport[baseIndex + row] &= ~(1 << row);
   WriteFrame(CurrentOutReport);
}

void Manta::SetLEDRow(LEDColor color, int row, uint8_t mask)
{
   int baseIndex;
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   
   switch(color)
   {
      case Amber:
         baseIndex = 0;
         break;
      case Red:
         baseIndex = 10;
         break;
      default:
         throw std::invalid_argument("Invalid Color");
   }
   if(row >= 6)
   {
      throw std::invalid_argument("Invalid Row Index");
   }
   CurrentOutReport[baseIndex + row] = byteReverse(mask);
   WriteFrame(CurrentOutReport);
}

void Manta::SetLEDColumn(LEDColor color, int column, uint8_t mask)
{
   int baseIndex;
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   
   switch(color)
   {
      case Amber:
         baseIndex = 0;
         break;
      case Red:
         baseIndex = 10;
         break;
      default:
         throw std::invalid_argument("Invalid Color");
   }
   if(column >= 8)
   {
      throw std::invalid_argument("Invalid Column Index");
   }

   for(int i = 0; i < 6; ++i)
   {
      if((mask >> i) & 0x01)
         CurrentOutReport[baseIndex + i] |= (0x01 << column);
      else
         CurrentOutReport[baseIndex + i] &= ~(0x01 << column);
   }
   WriteFrame(CurrentOutReport);
}

void Manta::SetLEDFrame(LEDColor color, LEDFrame mask)
{
   int baseIndex;
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   
   switch(color)
   {
      case Amber:
         baseIndex = 0;
         break;
      case Red:
         baseIndex = 10;
         break;
      default:
         throw std::invalid_argument("Invalid Color");
   }
   for(int i = 0; i < sizeof(LEDFrame); ++i)
   {
      CurrentOutReport[baseIndex + i] = byteReverse(mask[i]);
   }
   WriteFrame(CurrentOutReport);
}

void Manta::SetSliderLEDs(int id, uint8_t mask)
{
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   
   if(id > 1)
   {
      throw std::invalid_argument("Invalid Slider Index");
   }
   CurrentOutReport[id + 7] = mask;
   WriteFrame(CurrentOutReport);
}

void Manta::SetButtonLEDs(LEDColor color, int id, bool enabled)
{
   int shiftBase;
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   
   switch(color)
   {
      case Amber:
         shiftBase = 0;
         break;
      case Red:
         shiftBase = 4;
         break;
      default:
         throw std::invalid_argument("Invalid Color");
   }
   if(id > 3)
   {
      throw std::invalid_argument("Invalid Button Index");
   }
   if(enabled)
      CurrentOutReport[6] |= (0x01 << (id + shiftBase));
   else
      CurrentOutReport[6] &= ~(0x01 << (id + shiftBase));
   WriteFrame(CurrentOutReport);
}

void Manta::Recalibrate(void)
{
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   
   CurrentOutReport[9] |= 0x40;
   WriteFrame(CurrentOutReport);
   CurrentOutReport[9] &= ~0x40;
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
      CurrentOutReport[9] |= flag;
   else
      CurrentOutReport[9] &= ~flag;
   WriteFrame(CurrentOutReport);
}

void Manta::SetTurboMode(bool Enabled)
{
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   
   if(Enabled)
      CurrentOutReport[9] |= 0x04;
   else
      CurrentOutReport[9] &= ~0x04;
   WriteFrame(CurrentOutReport);
}

void Manta::SetRawMode(bool Enabled)
{
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   
   if(Enabled)
      CurrentOutReport[9] |= 0x08;
   else
      CurrentOutReport[9] &= ~0x08;
   WriteFrame(CurrentOutReport);
}

void Manta::SetHiResMode(bool Enabled)
{
   if(! IsConnected())
   {
      throw MantaNotConnectedException();
   }
   
   if(Enabled)
      CurrentOutReport[9] |= 0x10;
   else
      CurrentOutReport[9] &= ~0x10;
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
