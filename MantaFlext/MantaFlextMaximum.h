#include <flext.h>
#include <stdint.h>

class MantaFlextMaximum : public flext_base
{
   FLEXT_HEADER(MantaFlextMaximum, flext_base)
      
   public:
   MantaFlextMaximum();

   protected:
   void padFrameHandler(int argc, t_atom *argv);
   void skipHandler(int pad);
   void noskipHandler(int pad);
   void SetOneIndexed(int enabled);

   private:
   int lastMax;
   uint32_t skippedPadsMask[2];
   bool OneIndexed;
   FLEXT_CALLBACK_V(padFrameHandler)
   FLEXT_CALLBACK_F(skipHandler)
   FLEXT_CALLBACK_F(noskipHandler)
   FLEXT_CALLBACK_1(SetOneIndexed, int)
};
