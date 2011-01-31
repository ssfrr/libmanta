#include "../core/MantaUSB.h"
#include <iostream>
#include <cstring>

using namespace std;

class MantaUSBTester : public MantaUSB
{
   public:
      void PublicWriteFrame(uint8_t *frame)
      {
         WriteFrame(frame);
      }
   private:
   virtual void FrameReceived(int8_t *frame)
   {
      for(int i = 1; i < 53; ++i)
      {
         cout << (frame[i] + 128) << " ";
         if(i % 8 == 0)
         {
            cout << endl;
         }
      }
      cout << endl;
      cout << ((frame[53] + 128) | ((frame[54] + 128) << 8)) << endl;
      cout << ((frame[55] + 128) | ((frame[56] + 128) << 8)) << endl;
      cout << endl;
   }
};

int main()
{
   MantaUSBTester dev;
   uint8_t outbuf[16];
   
   dev.Connect();
   memset(outbuf, 0, 16);
   outbuf[9] |= 0x01 | 0x02;
   for(int i = 0; i < 52; ++i)
   {
      outbuf[i / 8] |= (1 << (i % 8));
      dev.PublicWriteFrame(outbuf);
      while(dev.IsTransmitting())
      {
         dev.HandleEvents();
      }
      usleep(100000);
      outbuf[i / 8] &= ~(1 << (i % 8));
   }
   for(int i = 0; i < 52; ++i)
   {
      outbuf[i / 8 + 10] |= (1 << (i % 8));
      dev.PublicWriteFrame(outbuf);
      while(dev.IsTransmitting())
      {
         dev.HandleEvents();
      }
      usleep(100000);
      outbuf[i / 8 + 10] &= ~(1 << (i % 8));
   }
   outbuf[7] = 1;
   while(outbuf[7])
   {
      dev.PublicWriteFrame(outbuf);
      while(dev.IsTransmitting())
      {
         dev.HandleEvents();
      }
      usleep(100000);
      outbuf[7] <<= 1;
   }
   outbuf[8] = 1;
   while(outbuf[8])
   {
      dev.PublicWriteFrame(outbuf);
      while(dev.IsTransmitting())
      {
         dev.HandleEvents();
      }
      usleep(100000);
      outbuf[8] <<= 1;
   }
   outbuf[9] = 0;
   dev.PublicWriteFrame(outbuf);
   while(dev.IsTransmitting())
   {
      dev.HandleEvents();
   }

   while(1)
   {
      dev.HandleEvents();
   }
   return 0;
}