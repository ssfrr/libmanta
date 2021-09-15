#include "../core/MantaUSB.h"
#include <iostream>
#include <cstring>
#include <cstdarg>
#include <cstdio>

using namespace std;

class MantaUSBTester : public MantaUSB
{
   public:
   void PublicWriteFrame(uint8_t *frame)
   {
      WriteFrame(frame, false);
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
   void DebugPrint(const char *fmt, ...)
   {
      va_list args;
      char string[256];
      va_start(args, fmt);
      vsprintf(string, fmt, args);
      va_end (args);
      cout << string << endl;
   }
};

int main()
{
   MantaUSBTester dev[2];
   uint8_t outbuf[16];
   
   dev[0].Connect(118);
   dev[1].Connect(58);
   memset(outbuf, 0, 16);
   outbuf[9] |= 0x01 | 0x02;
   /* light up all the yellow pad and button LEDs */
   for(int i = 0; i < 48; ++i)
   {
      outbuf[i / 8] |= (1 << (i % 8));
      dev[0].PublicWriteFrame(outbuf);
      dev[1].PublicWriteFrame(outbuf);
      MantaUSB::HandleEvents();
      MantaUSB::HandleEvents();
      usleep(100000);
      outbuf[i / 8] &= ~(1 << (i % 8));
   }
   for(int i = 0; i < 4; ++i)
   {
      outbuf[6] |= (1 << (i % 8));
      dev[0].PublicWriteFrame(outbuf);
      dev[1].PublicWriteFrame(outbuf);
      MantaUSB::HandleEvents();
      MantaUSB::HandleEvents();
      usleep(100000);
      outbuf[6] &= ~(1 << (i % 8));
   }
   /* light up all the red pad and button LEDs */
   for(int i = 0; i < 48; ++i)
   {
      outbuf[i / 8 + 10] |= (1 << (i % 8));
      dev[0].PublicWriteFrame(outbuf);
      dev[1].PublicWriteFrame(outbuf);
      MantaUSB::HandleEvents();
      MantaUSB::HandleEvents();
      usleep(100000);
      outbuf[i / 8 + 10] &= ~(1 << (i % 8));
   }
   for(int i = 0; i < 4; ++i)
   {
      outbuf[6] |= (1 << (i % 8 + 4));
      dev[0].PublicWriteFrame(outbuf);
      dev[1].PublicWriteFrame(outbuf);
      MantaUSB::HandleEvents();
      MantaUSB::HandleEvents();
      usleep(100000);
      outbuf[6] &= ~(1 << (i % 8 + 4));
   }
   outbuf[7] = 1;
   while(outbuf[7])
   {
      dev[0].PublicWriteFrame(outbuf);
      dev[1].PublicWriteFrame(outbuf);
      MantaUSB::HandleEvents();
      MantaUSB::HandleEvents();
      usleep(100000);
      outbuf[7] <<= 1;
   }
   outbuf[8] = 1;
   while(outbuf[8])
   {
      dev[0].PublicWriteFrame(outbuf);
      dev[1].PublicWriteFrame(outbuf);
      MantaUSB::HandleEvents();
      MantaUSB::HandleEvents();
      usleep(100000);
      outbuf[8] <<= 1;
   }
   outbuf[9] = 0;
   dev[0].PublicWriteFrame(outbuf);
   dev[1].PublicWriteFrame(outbuf);
   MantaUSB::HandleEvents();
   MantaUSB::HandleEvents();

   while(1)
   {
      MantaUSB::HandleEvents();
      usleep(1000);
   }
   return 0;
}
