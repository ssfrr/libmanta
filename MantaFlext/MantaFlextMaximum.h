#include <flext.h>

class MantaFlextMaximum : public flext_base
{
   FLEXT_HEADER(MantaFlextMaximum, flext_base)
      
   public:
   MantaFlextMaximum();

   protected:
   void padFrameHandler(int argc, t_atom *argv);

   private:
   int lastMax;
   FLEXT_CALLBACK_V(padFrameHandler)
};
