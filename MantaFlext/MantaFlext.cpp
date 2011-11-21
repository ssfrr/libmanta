#include "MantaFlext.h"
#include <algorithm>

class MantaFinder
{
   public:
      MantaFinder(int s) : serialToMatch(s) {}
      bool operator()(const MantaMulti *dev)
      {
         /*
         return dev->GetSerialNumber() == serial;
         */
         /*for now just return true. we'll need the conditional when we start
          * being able to select a specific serial number */
         return true;
      }
   private:
      int serialToMatch;
};

FLEXT_LIB("manta",manta)

manta::manta():
   ConnectedManta(NULL)
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
   FLEXT_ADDMETHOD_(1, "ledsoff", ClearPadAndButtonLEDs);
   FLEXT_ADDMETHOD_2(0, "ledcontrol", SetLEDControl, t_symptr, int);
   FLEXT_ADDMETHOD_(0, "reset", Recalibrate);
   FLEXT_ADDMETHOD_(0, "connect", StartThread);
   
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
   ledsOffSymbol = MakeSymbol("ledsoff");

   /* use flext to call the threaded method */
   FLEXT_CALLMETHOD(StartThread);
} 

manta::~manta()
{ 
   if(threadRunning)
   {
      Lock();
      shouldStop = true;
      /* Wait() unlocks the lock while waiting */
      cond.Wait();
      Unlock();
   }
   Detach();
} 

/* this function attaches the Mantaflext instance to a Manta instance, but does
 * not connect that Manta instance to an actual hardware manta */
void manta::Attach(int serialNumber)
{
   MantaFinder pred(serialNumber);
   list<MantaMulti *>::iterator deviceIter;
   MantaMulti *device;
   if(! Attached())
   {
      deviceIter = find_if(ConnectedMantaList.begin(), ConnectedMantaList.end(), pred);
      /* see if the device is already in the connected list */
      if(ConnectedMantaList.end() != deviceIter)
      {
         post("manta: there's a device in the Connected List");
         device = *deviceIter;
         device->AttachClient(this);
         ConnectedManta = device;
      }
      else
      {
         post("manta: there's no device in the Connected List, adding one");
         /* TODO: open by serial number */
         ConnectedManta = new MantaMulti(this);
         ConnectedMantaList.push_back(ConnectedManta);
      }
   }
   else
   {
      post("manta: already attached");
   }
}

void manta::Detach()
{
   connectionMutex.Lock();
   if(Attached())
   {
      ConnectedManta->DetachClient(this);
      if(0 == ConnectedManta->GetReferenceCount())
      {
         ConnectedManta->Disconnect();
         ConnectedMantaList.remove(ConnectedManta);
         delete ConnectedManta;
      }
      ConnectedManta = NULL;
   }
   connectionMutex.Unlock();
}

bool manta::Attached()
{
   return ConnectedManta != NULL;
}

void manta::PollConnectedManta(MantaMultiListEntry *mantaEntry)
{
   if(threadRunning)
   {
      post("manta: Already Connected");
      return;
   }
   threadRunning = true;
   try
   {
      connectionMutex.Lock();
      Attach();
      if(1 == ConnectedManta->GetReferenceCount())
      {
         ConnectedManta->Connect();
         post("manta: Connected to Manta %d", ConnectedManta->GetSerialNumber());
         ConnectedManta->ResendLEDState();
         connectionMutex.Unlock();
         while(!shouldStop)
         {
            /* ensure that only one thread is handling events at a time. This
             * is probably excessive, but much simpler than finer-grained locking */
            connectionMutex.Lock();
            ConnectedManta->HandleEvents();
            connectionMutex.Unlock();
         }
      }
      else
      {
         connectionMutex.Unlock();
      }
   }
   catch(MantaNotFoundException e)
   {
      Detach();
      connectionMutex.Unlock();
      post("manta: No attached Mantas found. Plug in a Manta and send \"connect\"");
   }
   catch(MantaOpenException e)
   {
      Detach();
      connectionMutex.Unlock();
      post("manta: Could not connect to attached Manta");
   }
   catch(MantaCommunicationException e)
   {
      Detach();
      connectionMutex.Unlock();
      post("manta: Communication with Manta interrupted");
   }
   threadRunning = false;
   Lock();
   cond.Signal();
   Unlock();
}

MantaMultiListEntry::MantaMultiListEntry() :
   mantaServer(NULL),
   shouldStop(false),
   threadRunning(false)
{
}

MantaMultiListEntry::~MantaMultiListEntry()
{
}

FLEXT_CLASSDEF(flext)::ThrMutex manta::connectionMutex;
list<MantaMultiListEntry *> manta::ConnectedMantaList;
