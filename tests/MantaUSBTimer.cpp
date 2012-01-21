#include "../core/MantaUSB.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include "sys/time.h"

using namespace std;

typedef struct tag_time_measure
{
   struct timeval startTimeVal;
   struct timeval stopTimeVal;
} time_measure;

void startTimeMeasuring(time_measure *tu)
{
   gettimeofday(&tu->startTimeVal,0);
}

void stopTimeMeasuring(time_measure * tu)
{
   gettimeofday(&tu->stopTimeVal,0);
}

void printMeasuredTime(time_measure * tu, int trials)
{
   struct timeval elapsedVal;
   double elapsed_millis = 0.0f;

   timersub(&tu->stopTimeVal, &tu->startTimeVal, &elapsedVal);
   elapsed_millis = elapsedVal.tv_sec * 1000 + (double) elapsedVal.tv_usec / 1000;
   printf("\nTotal Elapsed Time: %fms, Per-Trial Time: %fms\n", elapsed_millis, elapsed_millis / trials);
}

class MantaUSBTimer : public MantaUSB
{
   public:
      MantaUSBTimer()
      {
         newValue = false;
      }
      bool newValue;
   private:
      virtual void FrameReceived(int8_t *frame)
      {
         newValue = true;
      }
};

void printPercentDone(int count, int total)
{
   if(count % (total / 100) == 0)
   {
      printf("\r%02d%%", (total - count) * 100 / total);
      fflush(stdout);
   }
}

int main()
{
   MantaUSBTimer dev;
   const int trials = 1000;
   int i;
   uint8_t outFrame[16];
   time_measure tu;

   dev.Connect();

   printf("\nMeasuring HandleEvents Time\n");
   i = trials;
   startTimeMeasuring(&tu);
   while(i--)
   {
      dev.HandleEvents();
      printPercentDone(i, trials);
   }
   stopTimeMeasuring(&tu);
   printMeasuredTime(&tu, trials);

   printf("\nMeasuring Read Speed\n");
   i = trials;
   startTimeMeasuring(&tu);
   while(i)
   {
      if(dev.newValue)
      {
         dev.newValue = false;
         --i;
      }
      dev.HandleEvents();
      printPercentDone(i, trials);
   }
   stopTimeMeasuring(&tu);
   printMeasuredTime(&tu, trials);

   printf("\nMeasuring Write Speed\n");
   i = trials;
   startTimeMeasuring(&tu);
   while(i--)
   {
      while(dev.MessageQueued())
      {
         dev.HandleEvents();
      }
      dev.WriteFrame(outFrame);
      printPercentDone(i, trials);
   }
   stopTimeMeasuring(&tu);
   printMeasuredTime(&tu, trials);
   return 0;
}

int usecondsDiff(struct timespec current, struct timespec previous)
{
}
