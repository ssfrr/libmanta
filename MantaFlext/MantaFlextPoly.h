#include <flext.h>
#include <vector>
using namespace std;

class MantaFlextPoly : public flext_base
{
   FLEXT_HEADER(MantaFlextPoly, flext_base)
      
   public:
   MantaFlextPoly(int argc, t_atom *argv);

   protected:
   void PadHandler(int pad);
   void ValueHandler(int value);
   void PadAndValueHandler(int pad, int value);

   private:
   void SendOutput(int voice, int pad, int value);
   int GetVoice(unsigned int pad);
   int FindVoice(int pad) const;
   int GetUnallocatedVoiceNumber() const;
   void ReleaseVoice(unsigned int voice);
   int MaxVoices;
   int StoredValue;
   bool Stealing;
   vector<int> Voices;
   FLEXT_CALLBACK_I(PadHandler)
   FLEXT_CALLBACK_I(ValueHandler)
   FLEXT_CALLBACK_II(PadAndValueHandler)
};

/*
class MantaVoice
{
   MantaVoice(int voice, int pad) :
      VoiceNumber(voice),
      PadNumber(pad)
   {
   }
   int VoiceNumber;
   int PadNumber;
}
*/
