#include "MantaFlextMaximum.h"

FLEXT_LIB("manta.maximum", MantaFlextMaximum);

MantaFlextMaximum::MantaFlextMaximum() :
   lastMax(0),
   OneIndexed(false)
{
   skippedPadsMask[0] = 0;
   skippedPadsMask[1] = 0;
   AddInAnything();
   AddOutList();
   FLEXT_ADDMETHOD_V(0, padFrameHandler);
   FLEXT_ADDMETHOD_F(0, "skip", skipHandler);
   FLEXT_ADDMETHOD_F(0, "noskip", noskipHandler);
   FLEXT_ADDMETHOD_1(0, "oneindex", SetOneIndexed, int);
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
   if(0 == max)
   {
      /* if no pads are hit, send -1 for both */
      max = -1;
      maxIndex = -1;
   }
   else
   {
      maxIndex += (OneIndexed ? 1 : 0);
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
   if(OneIndexed)
   {
      pad -= 1;
   }
   if(pad >= 0 && pad <= 47)
   {
      skippedPadsMask[pad / 32] |= (0x01 << (pad % 32));
   }
}

void MantaFlextMaximum::noskipHandler(int pad)
{
   if(OneIndexed)
   {
      pad -= 1;
   }
   if(pad >= 0 && pad <= 47)
   {
      skippedPadsMask[pad / 32] &= ~(0x01 << (pad % 32));
   }
}

void MantaFlextMaximum::SetOneIndexed(int enabled)
{
   OneIndexed = enabled;
}
