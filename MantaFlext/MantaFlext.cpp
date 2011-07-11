#include "MantaFlext.h"
#include <cstdarg>
#include <cstdio>

FLEXT_LIB("manta",manta)

manta::manta():
   shouldStop(false)
{ 
	AddInAnything();
	AddInAnything();

	AddOutAnything("Pad Velocity");
	AddOutAnything("Pad and Button Continuous");
	AddOutAnything("Slider");
	AddOutAnything("Processed");

   FLEXT_ADDMETHOD_(1, "pad", SetPadLED);
   FLEXT_ADDMETHOD_3(1, "row", SetPadLEDRow, t_symptr, int, int);
   FLEXT_ADDMETHOD_3(1, "row", SetPadLEDRowNum, int, int, int);
   FLEXT_ADDMETHOD_3(1, "column", SetPadLEDColumn, t_symptr, int, int);
   FLEXT_ADDMETHOD_3(1, "column", SetPadLEDColumnNum, int, int, int);
   FLEXT_ADDMETHOD_2(1, "slider", SetSliderLED, int, t_symptr);
   FLEXT_ADDMETHOD_2(1, "slider", SetSliderLEDNum, int, int);
   FLEXT_ADDMETHOD_3(1, "slidermask", SetSliderLEDMask, t_symptr, int, int);
   FLEXT_ADDMETHOD_3(1, "slidermask", SetSliderLEDMaskNum, int, int, int);
   FLEXT_ADDMETHOD_(1, "button", SetButtonLED);
   FLEXT_ADDMETHOD_2(0, "ledcontrol", SetLEDControl, t_symptr, int);
   FLEXT_ADDMETHOD_(0, "reset", Recalibrate);
   
   padSymbol = MakeSymbol("pad");
   sliderSymbol = MakeSymbol("slider");
   sliderMaskSymbol = MakeSymbol("slidermask");
   buttonSymbol = MakeSymbol("button");
   amberSymbol = MakeSymbol("amber");
   offSymbol = MakeSymbol("off");
   redSymbol = MakeSymbol("red");
   rowSymbol = MakeSymbol("row");
   columnSymbol = MakeSymbol("column");
   frameSymbol = MakeSymbol("frame");
   padAndButtonSymbol = MakeSymbol("padandbutton");

   /* use flext to call the threaded method */
   FLEXT_CALLMETHOD(StartThread);
} 

manta::~manta()
{ 
   Lock();
   shouldStop = true;
   /* Wait() unlocks the lock while waiting */
   cond.Wait();
   Unlock();
} 

void manta::DebugPrint(const char *fmt, ...)
{
#if 0
   va_list args;
   char string[256];
   va_start(args, fmt);
   vsprintf(string, fmt, args);
   va_end (args);
   post(string);
#endif
}

#if 0
const t_symbol *manta::padSymbol;
const t_symbol *manta::buttonSymbol;
const t_symbol *manta::sliderSymbol;
const t_symbol *manta::amberSymbol;
const t_symbol *manta::redSymbol;
const t_symbol *manta::offSymbol;
const t_symbol *manta::rowSymbol;
const t_symbol *manta::columnSymbol;
const t_symbol *manta::frameSymbol;
const t_symbol *manta::padAndButtonSymbol;
#endif
