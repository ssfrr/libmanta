#include "MantaUSB.h"
#include <iostream>
#include <cstring>

using namespace std;

class MantaUSBTest : public MantaUSB
{
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
}

int main()
{
   MantaUSB dev;
   uint8_t inbuf[64];
   int8_t *signedBuf = reinterpret_cast<int8_t *>(inbuf);
   uint8_t outbuf[16];
   memset(outbuf, 0, 16);
   outbuf[9] |= 0x01 | 0x02;
   for(int i = 0; i < 52; ++i)
   {
      outbuf[i / 8] |= (1 << (i % 8));
      dev.WriteFrame(outbuf);
      usleep(100000);
      outbuf[i / 8] &= ~(1 << (i % 8));
   }
   for(int i = 0; i < 52; ++i)
   {
      outbuf[i / 8 + 10] |= (1 << (i % 8));
      dev.WriteFrame(outbuf);
      usleep(100000);
      outbuf[i / 8 + 10] &= ~(1 << (i % 8));
   }
   outbuf[7] = 1;
   while(outbuf[7])
   {
      dev.WriteFrame(outbuf);
      usleep(100000);
      outbuf[7] <<= 1;
   }
   outbuf[8] = 1;
   while(outbuf[8])
   {
      dev.WriteFrame(outbuf);
      usleep(100000);
      outbuf[8] <<= 1;
   }
   outbuf[9] = 0;
   dev.WriteFrame(outbuf);

   while(1)
   {
      dev.ReadFrame(inbuf);
      for(int i = 1; i < 53; ++i)
      {
         cout << (signedBuf[i] + 128) << " ";
         if(i % 8 == 0)
         {
            cout << endl;
         }
      }
      cout << endl;
      cout << ((signedBuf[53] + 128) | ((signedBuf[54] + 128) << 8)) << endl;
      cout << ((signedBuf[55] + 128) | ((signedBuf[56] + 128) << 8)) << endl;
      cout << endl;
   }
   return 0;
}
