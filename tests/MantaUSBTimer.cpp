#include "../core/MantaUSB.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include "time.h"

using namespace std;

class MantaUSBTimer : public MantaUSB
{
   public:
   void PublicWriteFrame(uint8_t *frame)
   {
      WriteFrame(frame);
   }
   int lastDiff;
   private:
   virtual void FrameReceived(int8_t *frame)
   {
      clock_gettime(CLOCK_MONOTONIC, &current);
      lastDiff = usecondsDiff();
      previous = current;
   }
   int usecondsDiff();
   struct timespec current;
   struct timespec previous;
};

int main()
{
   MantaUSBTimer dev;
   ofstream outFile;
   
   outFile.open("runtimes.txt");

   dev.Connect();

   while(1)
   {
      outFile << dev.lastDiff << endl;
      dev.HandleEvents();
   }
   outFile.close();
   return 0;
}

int MantaUSBTimer::usecondsDiff()
{
   int usDiff = (current.tv_sec - previous.tv_sec) * 1000000;
   usDiff += (current.tv_nsec - previous.tv_nsec) / 1000;
   return usDiff;
}
