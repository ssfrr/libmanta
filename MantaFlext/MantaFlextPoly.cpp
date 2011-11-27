#include "MantaFlextPoly.h"

FLEXT_LIB_V("manta.poly", MantaFlextPoly);

MantaFlextPoly::MantaFlextPoly(int argc, t_atom *argv) :
   StoredValue(-1)
{
   if(argc > 0 && CanbeInt(argv[0]))
   {
      /* Voices initialized to empty vector, so all items are new
       * and initialized to -1 */
      Voices.assign(GetInt(argv[0]), -1);
   }
   else
   {
      Voices.assign(48, -1);
   }
   /* voice stealing is not yet implemented */
   if(argc > 1 && CanbeInt(argv[1]))
   {
      Stealing = GetInt(argv[1]);
   }
   AddInFloat();
   AddInFloat();
   AddOutFloat();
   AddOutFloat();
   AddOutFloat();
   FLEXT_ADDMETHOD(0, PadHandler);
   FLEXT_ADDMETHOD(1, ValueHandler);
   FLEXT_ADDMETHOD(0, PadAndValueHandler);
}

void MantaFlextPoly::PadHandler(int pad)
{
   SendOutput(GetVoice(pad), pad, StoredValue);
   StoredValue = -1;
}

void MantaFlextPoly::ValueHandler(int value)
{
   StoredValue = value;
}

void MantaFlextPoly::PadAndValueHandler(int pad, int value)
{
   SendOutput(GetVoice(pad), pad, value);
   StoredValue = -1;
}

void MantaFlextPoly::SendOutput(int voice, int pad, int value)
{
   if(value >= 0 && voice >= 0)
   {
      ToOutFloat(2, value);
      ToOutFloat(1, pad);
      ToOutFloat(0, voice);
   }
   if(0 == value)
   {
      ReleaseVoice(voice);
   }
}

int MantaFlextPoly::GetVoice(unsigned int pad)
{
   int voice = FindVoice(pad);
   if(-1 == voice)
   {
      /* not in list, add it! */
      voice = GetUnallocatedVoiceNumber();
      if(voice >= 0)
      {
         Voices[voice] = pad;
      }
   }
   return voice;
}

int MantaFlextPoly::FindVoice(int pad) const
{
   for(unsigned int i = 0; i < Voices.size(); ++i)
   {
      if(Voices[i] == pad)
      {
         return i;
      }
   }
   return -1;
}

void MantaFlextPoly::ReleaseVoice(unsigned int voice)
{
   if(voice < Voices.size())
   {
      Voices[voice] = -1;
   }
}

int MantaFlextPoly::GetUnallocatedVoiceNumber() const
{
   return FindVoice(-1);
}
