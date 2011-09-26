#ifndef MANTAMULTI_H
#define MANTAMULTI_H

#include "Manta.h"
#include <list>

using namespace std;

class MantaMulti : public Manta
{
   public:
      MantaMulti(MantaClient *client = NULL);
      void AttachClient(MantaClient *client);
      void DetachClient(MantaClient *client);
      int GetReferenceCount();

   protected:
      void PadEvent(int row, int column, int id, int value);
      void SliderEvent(int id, int value);
      void ButtonEvent(int id, int value);
      void PadVelocityEvent(int row, int column, int id, int velocity);
      void ButtonVelocityEvent(int id, int velocity);
      void FrameEvent(int8_t *frame);
      void DebugPrint(const char *fmt, ...);
      
   private:
      list<MantaClient *> ClientList;
      int ReferenceCount;
};

#endif /* MANTAMULTI_H */
