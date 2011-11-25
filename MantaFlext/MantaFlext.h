#include <flext.h>
#include "../core/MantaClient.h"
#include "../core/MantaMulti.h"
#include <list>

using namespace std;

#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 400)
#error You need at least flext version 0.4.0
#endif

class manta:
	public flext_base,
   public MantaClient
{
	FLEXT_HEADER(manta, flext_base)
 
   public:
	manta(int argc,t_atom *argv); 
	~manta(); 

   void Attach(int serialNumber = 0);
   void Detach();
   bool Attached();
   void PadEvent(int row, int column, int id, int value);
   void SliderEvent(int id, int value);
   void ButtonEvent(int id, int value);
   void PadVelocityEvent(int row, int column, int id, int value);
   void ButtonVelocityEvent(int id, int value);
   void FrameEvent(int8_t *frame);
   void DebugPrint(const char *fmt, ...);

   protected:
   void SetPadLED(int argc, t_atom *argv);
   void SetPadLEDRow(t_symbol *state, int row, int mask);
   void SetPadLEDRowNum(int state, int row, int mask);
   void SetPadLEDColumn(t_symbol *state, int column, int mask);
   void SetPadLEDColumnNum(int state, int column, int mask);
   void SetSliderLEDMask(t_symbol *state, int id, int mask);
   void SetSliderLEDMaskNum(int state, int id, int mask);
   void SetSliderLED(int id, t_symbol *state);
   void SetSliderLEDNum(int id, int ledNum);
   void SetButtonLED(int argc, t_atom *argv);
   void SetLEDControl(t_symbol *control, int state);
   void Recalibrate();
   void ClearPadAndButtonLEDs();
   void SetTurboMode(int Enabled);
   void SetRawMode(int Enabled);

   private:
   LEDState ledStateFromSymbol(const t_symbol *stateSymbol);
   LEDState ledStateFromInt(int stateSymbol);

   /* we could be detached from the polling thread on communication error
    * so this SHOULD be volatile, but I got lazy */
   MantaMulti *ConnectedManta;

	static void PollConnectedMantas(thr_params *p);
   static MantaMulti *FindConnectedMantaBySerial(int serialNumber);
   static void DetachAllMantaFlext(MantaMulti *multi);

   //! Shared list of all connected mantas
   static list<MantaMulti *> ConnectedMantaList;
   static list<manta *> MantaFlextList;
   /* thread conditional to wait on to make sure
    * that the polling thread has stopped */
   static ThrCond ThreadRunningCond;
   static volatile bool shouldStop;
   static volatile bool threadRunning;
   /* shared mutex used to prevent connection-related race conditions */
   static ThrMutex MantaMutex;

   /* declare message handlers */
   FLEXT_CALLBACK_V(SetPadLED)
   FLEXT_CALLBACK_3(SetPadLEDRow, t_symptr, int, int)
   FLEXT_CALLBACK_3(SetPadLEDRowNum, int, int, int)
   FLEXT_CALLBACK_3(SetPadLEDColumn, t_symptr, int, int)
   FLEXT_CALLBACK_3(SetPadLEDColumnNum, int, int, int)
   FLEXT_CALLBACK_3(SetSliderLEDMask, t_symptr, int, int)
   FLEXT_CALLBACK_3(SetSliderLEDMaskNum, int, int, int)
   FLEXT_CALLBACK_2(SetSliderLED, int, t_symptr)
   FLEXT_CALLBACK_2(SetSliderLEDNum, int, int)
   FLEXT_CALLBACK_V(SetButtonLED)
   FLEXT_CALLBACK_2(SetLEDControl, t_symptr, int)
   FLEXT_CALLBACK(Recalibrate)
   FLEXT_CALLBACK(ClearPadAndButtonLEDs)
   FLEXT_CALLBACK_1(SetTurboMode, int)
   FLEXT_CALLBACK_1(SetRawMode, int)
   FLEXT_CALLBACK(Attach)

   int lastSliderValue[2];

   const t_symbol *padSymbol;
   const t_symbol *buttonSymbol;
   const t_symbol *sliderSymbol;
   const t_symbol *sliderMaskSymbol;
   const t_symbol *amberSymbol;
   const t_symbol *redSymbol;
   const t_symbol *offSymbol;
   const t_symbol *rowSymbol;
   const t_symbol *columnSymbol;
   const t_symbol *frameSymbol;
   const t_symbol *padAndButtonSymbol;
   const t_symbol *ledsOffSymbol;

   static const int velocityOutlet = 0;
   static const int continuousOutlet = 1;
   static const int sliderOutlet = 2;
   static const int frameOutlet = 3;
};

struct MantaMultiListEntry
{
   MantaMultiListEntry();
   ~MantaMultiListEntry();

   MantaMulti *mantaServer;
};
