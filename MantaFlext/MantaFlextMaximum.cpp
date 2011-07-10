#include "MantaFlextMaximum.h"

FLEXT_LIB("manta.maximum", MantaFlextMaximum);

MantaFlextMaximum::MantaFlextMaximum() :
   lastMax(0)
{
   skippedPadsMask[0] = 0;
   skippedPadsMask[1] = 0;
   AddInAnything();
   AddOutList();
   FLEXT_ADDMETHOD_V(0, padFrameHandler);
   FLEXT_ADDMETHOD_F(0, "skip", skipHandler);
   FLEXT_ADDMETHOD_F(0, "noskip", noskipHandler);
}

void MantaFlextMaximum::padFrameHandler(int argc, t_atom *argv)
{
   int max = -1;
   int maxIndex = -1;
   uint32_t currentMask = skippedPadsMask[0];
   for(int i = 0; i < argc; ++i)
   {
      if(32 == i)
      {
         currentMask = skippedPadsMask[1];
      }
      if(!(currentMask & 0x01) && GetFloat(argv[i]) > max)
      {
         max = GetFloat(argv[i]);
         maxIndex = i;
      }
      currentMask >>= 1;
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

void MantaFlextMaximum::skipHandler(int pad)
{
   skippedPadsMask[pad / 32] |= (0x01 << (pad % 32));
}

void MantaFlextMaximum::noskipHandler(int pad)
{
   skippedPadsMask[pad / 32] &= ~(0x01 << (pad % 32));
}
