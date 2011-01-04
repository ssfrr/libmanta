/* define FLEXT_THREADS for thread usage. Flext must also have been compiled with that defined!
	it's even better to define that as a compiler flag (-D FLEXT_THREADS) for all files of the
	flext external
*/
#ifndef FLEXT_THREADS
#define FLEXT_THREADS
#endif

#include <flext.h>
#include "Manta.h"

#if !defined(FLEXT_VERSION) || (FLEXT_VERSION < 400)
#error You need at least flext version 0.4.0
#endif


/* TODO:
 * handler functions should be virtual functions of the Manta class,
 * which manta should derive and implement
 */
class manta:
	public flext_base,
   public Manta
{
	FLEXT_HEADER(manta,flext_base)
 
public:
	manta(); 

protected:
	void m_startPoll();
	void m_stopPoll();
   void PadEvent(int id, int value);
   void SliderEvent(int id, int value);
   void ButtonEvent(int id, int value);

private:
	// define threaded callback for method m_start
	// the same syntax as with FLEXT_CALLBACK is used here
	FLEXT_THREAD(m_startPoll)
   FLEXT_CALLBACK(m_stopPoll)
};

FLEXT_NEW("manta",manta)

manta::manta()
{ 
	AddInAnything();
	AddOutAnything();
	AddOutFloat();
	AddOutFloat();

	FLEXT_ADDMETHOD_(0,"start", m_startPoll); // register method
	FLEXT_ADDMETHOD_(0,"stop", m_stopPoll); // register method
} 

void manta::m_startPoll()
{
   StartPoll();
}

void manta::m_stopPoll()
{
   StopPoll();
}
void manta::PadEvent(int id, int value)
{
   ToOutString(0, "pad");
   ToOutFloat(1, id);
   ToOutFloat(2, value);
}

void manta::SliderEvent(int id, int value)
{
   ToOutString(0, "slider");
   ToOutFloat(1, id);
   ToOutFloat(2, value);
}

void manta::ButtonEvent(int id, int value)
{
   ToOutString(0, "button");
   ToOutFloat(1, id);
   ToOutFloat(2, value);
}
