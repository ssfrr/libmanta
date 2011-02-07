#include <flext.h>
#include "../core/Manta.h"

#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 400)
#error You need at least flext version 0.4.0
#endif

class manta:
	public flext_base,
   public Manta
{
	FLEXT_HEADER(manta,flext_base)
 
public:
	manta(); 
	~manta(); 

protected:
	void m_connect();
	void m_disconnect();

private:
   void PadEvent(int id, int value);
   void SliderEvent(int id, int value);
   void ButtonEvent(int id, int value);
   void PadVelocityEvent(int id, int value);
   void ButtonVelocityEvent(int id, int value);
	// define threaded callback 
	// the same syntax as with FLEXT_CALLBACK is used here
	FLEXT_THREAD(m_connect)
   FLEXT_CALLBACK(m_disconnect)
   /* thread conditional to stop polling */

   int lastSliderValue[2];
   ThrCond cond;
   volatile bool shouldStop;
   volatile bool running;

   static const int padCode = 1;
   static const int buttonCode = 2;

   static const int onOffOutlet = 0;
   static const int velocityOutlet = 1;
   static const int continuousOutlet = 2;
   static const int sliderOutlet = 3;
   static const int maximumOutlet = 4;
   static const int centroidOutlet = 5;
};

FLEXT_NEW("manta",manta)

manta::manta():
   shouldStop(false),
   running(false)
{ 
	AddInAnything();

	AddOutAnything("Pad and Button On/Off");
	AddOutAnything("Pad Velocity");
	AddOutAnything("Pad and Button Continuous");
	AddOutAnything("Slider");
	AddOutAnything("Maximum Pad");
	AddOutAnything("Centroid");

   /* use flext to call the threaded method */
   FLEXT_CALLMETHOD(m_connect);
} 

manta::~manta()
{ 
   m_disconnect();
} 

void manta::m_connect()
{
   Connect();
   if(running)
   {
      return;
   }
   running = true;
   while(!ShouldExit() && !shouldStop)
   {
      HandleEvents();
   }
   running = false;
   cond.Signal();
}

void manta::m_disconnect()
{
   shouldStop = true;
   while(running)
   {
      cond.Wait();
   }
   shouldStop = false;
}

void manta::PadEvent(int id, int value)
{
   t_atom args[3];
   SetFloat(args[0], padCode);
   SetFloat(args[1], id + 1);
   SetFloat(args[2], value);
   ToOutList(continuousOutlet, sizeof(args) / sizeof(t_atom), args);
}

void manta::ButtonEvent(int id, int value)
{
   t_atom args[3];
   SetFloat(args[0], buttonCode);
   SetFloat(args[1], id + 49);
   SetFloat(args[2], value);
   ToOutList(continuousOutlet, sizeof(args) / sizeof(t_atom), args);
}

void manta::SliderEvent(int id, int value)
{
   t_atom args[3];
   SetFloat(args[0], id + 1);
   SetFloat(args[1], (value != 0xFFFF) ? value : lastSliderValue[id]);
   SetFloat(args[2], (value != 0xFFFF) ? 1 : 0);
   ToOutList(sliderOutlet, sizeof(args) / sizeof(t_atom), args);
   lastSliderValue[id] = value;
}

void manta::PadVelocityEvent(int id, int value)
{
   t_atom args[3];
   SetFloat(args[0], padCode);
   SetFloat(args[1], id + 1);
   SetFloat(args[2], value);
   ToOutList(velocityOutlet, sizeof(args) / sizeof(t_atom), args);

   SetFloat(args[2], value ? 1 : 0);
   ToOutList(onOffOutlet, sizeof(args) / sizeof(t_atom), args);
}

void manta::ButtonVelocityEvent(int id, int value)
{
   t_atom args[3];
   SetFloat(args[0], buttonCode);
   SetFloat(args[1], id + 49);
   SetFloat(args[2], value);
   ToOutList(velocityOutlet, sizeof(args) / sizeof(t_atom), args);

   SetFloat(args[2], value ? 1 : 0);
   ToOutList(onOffOutlet, sizeof(args) / sizeof(t_atom), args);
}
