#include <flext.h>
#include "../core/Manta.h"

#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 400)
#error You need at least flext version 0.4.0
#endif

class manta:
	public flext_base,
   public Manta
{
	FLEXT_HEADER_S(manta, flext_base, ClassSetup)
 
public:
	manta(); 
	~manta(); 

protected:
   static void ClassSetup(t_classid c);
	void StartThread();
   void SetPadLED(t_symbol *state, int ledID);
   void SetPadLEDRow(t_symbol *state, int row, int mask);
   void SetPadLEDColumn(t_symbol *state, int column, int mask);
   //void SetPadLEDFrame(t_symbol *state, int mask[]);
   void SetSliderLED(t_symbol *state, int id, int mask);
   void SetButtonLED(t_symbol *state, int id);
   void SetLEDControl(t_symbol *control, int state);
   /*
   void Recalibrate(void);
   void SetTurboMode(bool Enabled);
   void SetRawMode(bool Enabled);
   */

private:
   void PadEvent(int id, int value);
   void SliderEvent(int id, int value);
   void ButtonEvent(int id, int value);
   void PadVelocityEvent(int id, int value);
   void ButtonVelocityEvent(int id, int value);
   void MaximumEvent(int id, int value);
   void DebugPrint(const char *fmt, ...);

   LEDState ledStateFromSymbol(t_symbol *stateSymbol);
	// declare threaded callback 
	// the same syntax as with FLEXT_CALLBACK is used here
	FLEXT_THREAD(StartThread)
   /* declare message handlers */
   FLEXT_CALLBACK_2(SetPadLED, t_symptr, int)
   FLEXT_CALLBACK_3(SetPadLEDRow, t_symptr, int, int)
   FLEXT_CALLBACK_3(SetPadLEDColumn, t_symptr, int, int)
   FLEXT_CALLBACK_3(SetSliderLED, t_symptr, int, int)
   FLEXT_CALLBACK_2(SetButtonLED, t_symptr, int)
   FLEXT_CALLBACK_2(SetLEDControl, t_symptr, int)

   int lastSliderValue[2];
   /* thread conditional to wait on to make sure
    * that the polling thread has stopped */
   ThrCond cond;
   volatile bool shouldStop;

   static const t_symbol *padSymbol;
   static const t_symbol *buttonSymbol;
   static const t_symbol *sliderSymbol;
   static const t_symbol *amberSymbol;
   static const t_symbol *redSymbol;
   static const t_symbol *offSymbol;
   static const t_symbol *rowSymbol;
   static const t_symbol *columnSymbol;
   static const t_symbol *frameSymbol;
   static const t_symbol *padAndButtonSymbol;
   static const t_symbol *maxSymbol;

   static const int velocityOutlet = 0;
   static const int continuousOutlet = 1;
   static const int sliderOutlet = 2;
   static const int processedOutlet = 3;
};
