#include <cmath>
#include "Manta.h"
#include "MantaExceptions.h"

Manta::Manta(void) {
   for(int i = 0; i < 53; ++i)
   {
      LastInReport[i] = 0;
      MaxSensorValues[i] = AverageMaxSensorValues[i];
   }
   for(int i = 53; i < 57; ++i)
   {
      LastInReport[i] = 0xFF;
   }
   for(unsigned int i = 0; i < sizeof(CurrentOutReport); ++i)
   {
      CurrentOutReport[i] = 0;
   }
   for(unsigned int i = 0; i < sizeof(VelocityWaiting) / sizeof(VelocityWaiting[0]); ++i)
   {
      VelocityWaiting[i] = false;
   }
}

void Manta::FrameReceived(int8_t *frame)
{
   uint8_t *uframe = (uint8_t *)frame;
   for(int i = 1; i < 53; ++i)
   {
      uframe[i] = ScaleSensorValue(frame[i] + 128, i);
   }
   /* apply the offset to the slider bytes without scaling them */
   for(int i = 53; i < 57; ++i)
   {
      uframe[i] = frame[i] + 128;
   }
   FrameEvent(uframe);
   /* input frames have one reportID byte at the beginning */
   for(int i = 1; i < 53; ++i)
   {
      /* check to see if there's a previous sample waiting to have
       * the velocity algorithm run */
      if(true == VelocityWaiting[i])
      {
         if(i < 49)
            PadVelocityEvent((i - 1) / 8, (i - 1) % 8, i - 1,
                  CalculateVelocity(LastInReport[i], uframe[i]));
         else
            ButtonVelocityEvent(i - 49, CalculateVelocity(LastInReport[i], uframe[i]));
         VelocityWaiting[i] = false;
      }

      if(uframe[i] != LastInReport[i])
      {
         /* check to see if this is a release */
         if(0 == uframe[i])
         {
            if(i < 49)
               PadVelocityEvent((i - 1) / 8, (i - 1) % 8, i - 1, 0);
            else
               ButtonVelocityEvent(i - 49, 0);
         }
         /* check to see if this is the first nonzero sample */
         else if(0 == LastInReport[i])
         {
            VelocityWaiting[i] = true;
         }
         if(i < 49)
            PadEvent((i - 1) / 8, (i - 1) % 8, i - 1, uframe[i]);
         else
            ButtonEvent(i - 49, uframe[i]);
      }
      LastInReport[i] = uframe[i];
   }
   if(uframe[53] != LastInReport[53] || uframe[54] != LastInReport[54])
   {
      SliderEvent(0, (uframe[53]) | ((uframe[54]) << 8 ));
   }
   if(uframe[55] != LastInReport[55] || uframe[56] != LastInReport[56])
   {
      SliderEvent(1, (uframe[55]) | ((uframe[56]) << 8 ));
   }
   for(int i = 53; i < 57; ++i)
   {
      LastInReport[i] = uframe[i];
   }
}

void Manta::SetPadLED(LEDState state, int ledID)
{
   int row = ledID / 8;
   int column = ledID % 8;

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

   if(IsConnected())
   {
      WriteFrame(CurrentOutReport, false);
   }
}

void Manta::SetPadLEDRow(LEDState state, int row, uint8_t mask)
{
   if(row < 0 || row > 5)
   {
      throw std::invalid_argument("Invalid Row Index");
   }

   MantaClient::DebugPrint("Called SetPadLEDRow(%s, %d, %X)",
         state == Off ? "Off" : state == Amber ? "Amber" : "Red", row, mask);
   MantaClient::DebugPrint("ByteReverse(0x%X) = 0x%X", 0xA0, byteReverse(0xA0));
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
   if(IsConnected())
   {
      WriteFrame(CurrentOutReport, false);
   }
}

void Manta::SetPadLEDColumn(LEDState state, int column, uint8_t mask)
{
   if(column < 0 || column > 7)
   {
      throw std::invalid_argument("Invalid Column Index");
   }
   
   MantaClient::DebugPrint("Called SetPadLEDColumn(%s, %d, %X)",
         state == Off ? "Off" : state == Amber ? "Amber" : "Red", column, mask);
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

   if(IsConnected())
   {
      WriteFrame(CurrentOutReport, false);
   }
}

void Manta::SetPadLEDFrame(LEDState state, LEDFrame mask)
{
   switch(state)
   {
      case Amber:
         for(unsigned int i = 0; i < sizeof(LEDFrame); ++i)
         {
            CurrentOutReport[AmberIndex + i] = byteReverse(mask[i]);
            CurrentOutReport[RedIndex + i] &= ~byteReverse(mask[i]);
         }
         break;
      case Red:
         for(unsigned int i = 0; i < sizeof(LEDFrame); ++i)
         {
            CurrentOutReport[RedIndex + i] = byteReverse(mask[i]);
            CurrentOutReport[AmberIndex + i] &= ~byteReverse(mask[i]);
         }
         break;
      case Off:
         for(unsigned int i = 0; i < sizeof(LEDFrame); ++i)
         {
            CurrentOutReport[RedIndex + i] &= ~byteReverse(mask[i]);
            CurrentOutReport[AmberIndex + i] &= ~byteReverse(mask[i]);
         }
         break;
      default:
         throw std::invalid_argument("Invalid state");
   }
   if(IsConnected())
   {
      WriteFrame(CurrentOutReport, false);
   }
}

void Manta::SetSliderLED(LEDState state, int id, uint8_t mask)
{
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
   if(IsConnected())
   {
      WriteFrame(CurrentOutReport, false);
   }
}

void Manta::SetButtonLED(LEDState state, int id)
{
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
   if(IsConnected())
   {
      WriteFrame(CurrentOutReport, false);
   }
}

void Manta::ResendLEDState(void)
{
   if(IsConnected())
   {
      WriteFrame(CurrentOutReport, false);
   }
}

void Manta::ClearButtonLEDs(void)
{
   CurrentOutReport[ButtonIndex] = 0;

   if(IsConnected())
   {
      WriteFrame(CurrentOutReport, false);
   }
}

void Manta::ClearPadAndButtonLEDs(void)
{
   for(unsigned int i = 0; i < sizeof(LEDFrame); ++i)
   {
      CurrentOutReport[AmberIndex + i] = 0;
      CurrentOutReport[RedIndex + i] = 0;
   }

   CurrentOutReport[ButtonIndex] = 0;

   if(IsConnected())
   {
      WriteFrame(CurrentOutReport, false);
   }
}

void Manta::Recalibrate(void)
{
   if(! IsConnected())
   {
      throw MantaNotConnectedException(this);
   }
   
   /* make sure these messages get queued so that they
    * don't just cancel each other out */
   CurrentOutReport[ConfigIndex] |= 0x40;
   WriteFrame(CurrentOutReport, true);
   CurrentOutReport[ConfigIndex] &= ~0x40;
   WriteFrame(CurrentOutReport, true);
}

void Manta::SetLEDControl(LEDControlType control, bool state)
{
   uint8_t flag;
   
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
   if(IsConnected())
   {
      /* if we're disabling LEDControl, we want to make sure that this
       * message gets queued so that any pending LED messages get sent
       * down before we disable LEDs */
      WriteFrame(CurrentOutReport, !state);
   }
}

void Manta::SetTurboMode(bool Enabled)
{
   if(Enabled)
      CurrentOutReport[ConfigIndex] |= 0x04;
   else
      CurrentOutReport[ConfigIndex] &= ~0x04;
   if(IsConnected())
   {
      WriteFrame(CurrentOutReport, false);
   }
}

void Manta::SetRawMode(bool Enabled)
{
   if(Enabled)
      CurrentOutReport[ConfigIndex] |= 0x08;
   else
      CurrentOutReport[ConfigIndex] &= ~0x08;
   if(IsConnected())
   {
      WriteFrame(CurrentOutReport, false);
   }
}

void Manta::SetMaxSensorValues(int *values)
{
   for(int i = 0; i < 53; ++i)
   {
      MaxSensorValues[i] = values[i];
   }
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
   return outByte;
}

int Manta::CalculateVelocity(int LastValue, int CurrentValue)
{
   float LOG1, LOG2;
   float MAX;
   float MIN;
   float RELATIVE1, RELATIVE2;
   float LOG_RELATIVE1, LOG_RELATIVE2;
   float SUM_RAW;
   float LOG_SUM_RAW;
   float LOG_SUM_RELATIVE;
   float UP1;
   float VELOCITY = 0;
   int VELint = 0;


   // now do the velocity calculation
   LOG1 = log(1.0 + (float)LastValue);
   LOG2 = log(1.0 + (float)CurrentValue);

   MIN = LastValue;
   if (CurrentValue < MIN)
   {
      MIN = CurrentValue;
   }
   MAX = LastValue;
   if (CurrentValue > MAX)
   {
      MAX = CurrentValue;
   }
   RELATIVE1 = LastValue/MAX;
   RELATIVE2 = CurrentValue/MAX;
   LOG_RELATIVE1 = log(1.0 + RELATIVE1);
   LOG_RELATIVE2 = log(1.0 + RELATIVE2);
   SUM_RAW = LastValue+CurrentValue;
   LOG_SUM_RAW = log(1.0 + SUM_RAW);
   LOG_SUM_RELATIVE = log(1.0 + SUM_RAW/MAX);
   UP1 = 0;
   if (CurrentValue>LastValue) { UP1 = 1; }
   VELOCITY =
      -14.997037  +
      LastValue      *  0.009361  +
      MIN           * -0.014234  +
      LOG1          *  1.099763  +
      RELATIVE2     * -9.588311  +
      LOG_RELATIVE1  *-27.595303  +
      LOG_RELATIVE2  * -8.803761  +
      LOG_SUM_RELATIVE * 44.013138  +
      UP1           *  0.221622;
   //Then trim value to [0.4] range:
   if (VELOCITY < 0.)
   {
      VELOCITY = 0.;
   }
   if (VELOCITY > 4.)
   {
      VELOCITY = 4.;
   }
   //get it to 0. to 1. range
   VELOCITY = VELOCITY / 4.;
   // curve it exponentially
   VELOCITY = VELOCITY * VELOCITY;
   //get it to 0-126 range
   VELOCITY = VELOCITY * 126;
   //get it to 1-127 range
   VELOCITY = VELOCITY+ 1;
   //round to ints
   VELint = (int)VELOCITY;
   return VELint;
}

int Manta::ScaleSensorValue(int rawValue, int index)
{
   float div = (float)rawValue / MaxSensorValues[index];
   return (int)((div * 210) + 0.5);
}

const int Manta::AverageMaxSensorValues[53] = 
{0, 177, 184, 188, 189, 191, 190, 181, 181, 188, 193, 198, 200, 201, 199, 191,
   189, 192, 197, 202, 205, 206, 204, 199, 192, 202, 207, 211, 216, 215, 213,
   209, 201, 205, 210, 215, 220, 213, 218, 212, 204, 212, 216, 222, 227, 223,
   227, 221, 213, 200, 170, 190, 185};
