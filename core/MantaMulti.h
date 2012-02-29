#ifndef MANTAMULTI_H
#define MANTAMULTI_H

#include "Manta.h"
#include <list>

using namespace std;

/************************************************************************//**
 * \class MantaMulti
 * \brief Superclass that adds functionality for multiple access to a single
 *        Manta
 *
 * This class can be used by an application that wants to have multiple
 * MantaClients that connect to a single Manta using the MantaServer API.
 * If you only need single-access you should use the Manta class instead
 ****************************************************************************/
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
      void FrameEvent(uint8_t *frame);
      //void DebugPrint(const char *fmt, ...);
      
   private:
      list<MantaClient *> ClientList;
      int ReferenceCount;
};

#endif /* MANTAMULTI_H */
