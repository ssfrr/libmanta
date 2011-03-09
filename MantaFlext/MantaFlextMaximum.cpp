#include "MantaFlextMaximum.h"

FLEXT_LIB("manta.maximum", MantaFlextMaximum);

MantaFlextMaximum::MantaFlextMaximum() :
   lastMax(0)
{
   AddInAnything();
   AddOutList();
   FLEXT_ADDMETHOD_V(0, padFrameHandler);
}

void MantaFlextMaximum::padFrameHandler(int argc, t_atom *argv)
{
   int max = -1;
   int maxIndex = -1;
   for(int i = 0; i < argc; ++i)
   {
      if(GetFloat(argv[i]) > max)
      {
         max = GetFloat(argv[i]);
         maxIndex = i;
      }
   }
   if(max != lastMax)
   {
      t_atom args[2];
      SetFloat(args[0], maxIndex);
      SetFloat(args[1], max);
      lastMax = max;
      ToOutList(0, sizeof(args) / sizeof(t_atom), args);
   }
}
