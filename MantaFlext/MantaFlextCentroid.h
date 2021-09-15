#include <flext.h>

class MantaFlextCentroid : public flext_base
{
   FLEXT_HEADER(MantaFlextCentroid, flext_base)
      
   public:
   MantaFlextCentroid();

   protected:
   void padFrameHandler(int argc, t_atom *argv);

   private:
   bool dormant;
   FLEXT_CALLBACK_V(padFrameHandler)
};
