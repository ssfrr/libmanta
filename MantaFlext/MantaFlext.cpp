#include "MantaFlext.h"
#include "../core/MantaExceptions.h"
#include <algorithm>

FLEXT_LIB_V("manta",manta)

manta::manta(int argc,t_atom *argv):
   ConnectedManta(NULL),
   DebugEnabled(false),
   OneIndexed(false)
{ 
	AddInAnything();

	AddOutAnything("Pad Velocity");
	AddOutAnything("Pad and Button Continuous");
	AddOutAnything("Slider");
	AddOutAnything("Processed");

   FLEXT_ADDMETHOD_(0, "pad", SetPadLED);
   FLEXT_ADDMETHOD_3(0, "row", SetPadLEDRow, t_symptr, int, int);
   FLEXT_ADDMETHOD_3(0, "row", SetPadLEDRowNum, int, int, int);
   FLEXT_ADDMETHOD_3(0, "column", SetPadLEDColumn, t_symptr, int, int);
   FLEXT_ADDMETHOD_3(0, "column", SetPadLEDColumnNum, int, int, int);
   FLEXT_ADDMETHOD_2(0, "slider", SetSliderLED, int, t_symptr);
   FLEXT_ADDMETHOD_2(0, "slider", SetSliderLEDNum, int, int);
   FLEXT_ADDMETHOD_3(0, "slidermask", SetSliderLEDMask, t_symptr, int, int);
   FLEXT_ADDMETHOD_3(0, "slidermask", SetSliderLEDMaskNum, int, int, int);
   FLEXT_ADDMETHOD_(0, "button", SetButtonLED);
   FLEXT_ADDMETHOD_(0, "ledsoff", ClearPadAndButtonLEDs);
   FLEXT_ADDMETHOD_2(0, "ledcontrol", SetLEDControl, t_symptr, int);
   FLEXT_ADDMETHOD_(0, "reset", Recalibrate);
   FLEXT_ADDMETHOD_(0, "connect", Connect);
   FLEXT_ADDMETHOD_1(0, "turbo", SetTurboMode, int);
   FLEXT_ADDMETHOD_1(0, "raw", SetRawMode, int);
   FLEXT_ADDMETHOD_1(0, "debug", EnableDebug, int);
   FLEXT_ADDMETHOD_1(0, "oneindex", SetOneIndexed, int);
   
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

   MantaFlextList.push_back(this);
   Connect(argc, argv);
} 

manta::~manta()
{ 
   Detach();
   MantaFlextList.remove(this);
} 

/* this function attaches the Mantaflext instance to a Manta instance, but does
 * not connect that Manta instance to an actual hardware manta */
void manta::Attach(int serialNumber)
{
   MantaMutex.Lock();
   if(! Attached())
   {
      MantaMulti *device = FindConnectedMantaBySerial(serialNumber);
      /* see if the device is already in the connected list */
      if(NULL != device)
      {
         post("manta: Attaching to manta %d", device->GetSerialNumber());
         device->AttachClient(this);
         ConnectedManta = device;
         MantaMutex.Unlock();
      }
      else
      {
         MantaMutex.Unlock();
         /* Stop the polling thread while we add a manta */
         StopThreadAndWait();
         /* TODO: open by serial number */
         device = new MantaMulti;
         try
         {
            device->Connect(serialNumber);
            MantaMutex.Lock();
            post("manta: Connected to manta %d, attaching...", device->GetSerialNumber());
            device->AttachClient(this);
            device->ResendLEDState();
            ConnectedManta = device;
            ConnectedMantaList.push_back(ConnectedManta);
            MantaMutex.Unlock();
         }
         catch(MantaNotFoundException e)
         {
            post("manta: could not find matching manta");
            delete device;
         }
         catch(MantaOpenException e)
         {
            post("manta: Could not connect to attached Manta");
            delete device;
         }

         if(! ConnectedMantaList.empty())
         {
            StartThread();
         }
      }
   }
   else
   {
      MantaMutex.Unlock();
      post("manta: already attached");
   }
}

void manta::Detach()
{
   /* Stop the polling thread while we mess with the connection list */
   StopThreadAndWait();
   if(Attached())
   {
      post("manta: Detaching from manta %d", ConnectedManta->GetSerialNumber());
      if(ConnectedManta->GetReferenceCount() == 1)
      {
         /* TODO: if the polling thread is still running here and the callbacks
          * get called for the cancelled USB transfers, there will probably
          * be a segfault */
         post("manta: no more connections to manta %d, destroying.",ConnectedManta->GetSerialNumber());
         delete ConnectedManta;
         ConnectedMantaList.remove(ConnectedManta);
         ConnectedManta = NULL;
      }
      else
      {
         /* There are still other MantaFlext instances connected to this MantaMulti,
          * so just detach ourselves */
         ConnectedManta->DetachClient(this);
         ConnectedManta = NULL;
      }
   }
   if(! ConnectedMantaList.empty())
   {
      StartThread();
   }
}

bool manta::Attached()
{
   return ConnectedManta != NULL;
}

void manta::PollConnectedMantas(thr_params *p)
{
   if(threadRunning)
   {
      post("manta: Thread Already Running!");
      return;
   }
   if(ConnectedMantaList.empty())
   {
      post("manta: Polling thread started with no connected mantas");
      return;
   }
   threadRunning = true;
   while(!shouldStop && !ConnectedMantaList.empty())
   {
      try
      {
         MantaMutex.Lock();
         MantaUSB::HandleEvents();
         MantaMutex.Unlock();
         Sleep(0.002);
      }
      catch(MantaCommunicationException e)
      {
         MantaMulti *errorManta = static_cast<MantaMulti *>(e.errorManta);
         post("manta: Communication with Manta %d interrupted", errorManta->GetSerialNumber());
         delete errorManta;
         DetachAllMantaFlext(errorManta);
         ConnectedMantaList.remove(errorManta);
         MantaMutex.Unlock();
      }
   }
   ThreadRunningCond.Lock();
   threadRunning = false;
   ThreadRunningCond.Signal();
   ThreadRunningCond.Unlock();
}

/* make sure you're not holding another lock while calling this function,
 * or a deadlock may occur */
void manta::StopThreadAndWait()
{
   shouldStop = true;
   // We really should lock and unlock around the condition check,
   // but flext won't let us
   // ThreadRunningCond.Lock();
   while(threadRunning)
      ThreadRunningCond.Wait();
   // ThreadRunningCond.Unlock();
}

void manta::StartThread()
{
   shouldStop = false;
   /* start the polling thread if this is the first connected Manta */
   LaunchThread(PollConnectedMantas, NULL);
}

MantaMulti *manta::FindConnectedMantaBySerial(int serialNumber)
{
   list<MantaMulti *>::iterator i = ConnectedMantaList.begin();
   while(i != ConnectedMantaList.end())
   {
      if(serialNumber == 0 || (*i)->GetSerialNumber() == serialNumber)
      {
         return *i;
      }
      ++i;
   }
   return NULL;
}

void manta::DetachAllMantaFlext(MantaMulti *multi)
{
   list<manta *>::iterator i = MantaFlextList.begin();
   while(MantaFlextList.end() != i)
   {
      if((*i)->ConnectedManta == multi)
      {
         (*i)->ConnectedManta = NULL;
      }
      ++i;
   }
}

FLEXT_CLASSDEF(flext)::ThrMutex manta::MantaMutex;
list<MantaMulti *> manta::ConnectedMantaList;
list<manta *> manta::MantaFlextList;
FLEXT_CLASSDEF(flext)::ThrCond manta::ThreadRunningCond;
volatile bool manta::shouldStop = false;
volatile bool manta::threadRunning = false;
