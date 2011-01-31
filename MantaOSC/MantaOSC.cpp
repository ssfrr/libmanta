#include "../core/Manta.h"
#include "../core/MantaExceptions.h"
#include <lo/lo.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <stdexcept>

class MantaOSC : public Manta
{
   public:
      MantaOSC();
      ~MantaOSC();
   private:
      virtual void PadEvent(int id, int value);
      virtual void SliderEvent(int id, int value);
      virtual void ButtonEvent(int id, int value);
      friend int LEDControlHandler(const char *path, const char *types,
            lo_arg **argv, int argc, void *data, void *instancePointer);
      friend int LEDRowAndColumnHandler(const char *path,
            const char *types, lo_arg **argv, int argc, void *data,
            void *instancePointer);
      lo_address OSCAddress;
      lo_server_thread OSCServerThread;
};

void ErrorHandler(int num, const char *m, const char *path);
int LEDControlHandler(const char *path, const char *types,
      lo_arg **argv, int argc, void *data, void *instancePointer);
int LEDPadHandler(const char *path,
      const char *types, lo_arg **argv, int argc, void *data,
      void *instancePointer);
int LEDRowHandler(const char *path,
      const char *types, lo_arg **argv, int argc, void *data,
      void *instancePointer);
int LEDColumnHandler(const char *path,
      const char *types, lo_arg **argv, int argc, void *data,
      void *instancePointer);
int LEDFrameHandler(const char *path,
      const char *types, lo_arg **argv, int argc, void *data,
      void *instancePointer);
int LEDSliderHandler(const char *path,
      const char *types, lo_arg **argv, int argc, void *data,
      void *instancePointer);
int LEDButtonHandler(const char *path,
      const char *types, lo_arg **argv, int argc, void *data,
      void *instancePointer);

MantaOSC::LEDState getLEDStateFromString(const char *stateString);

MantaOSC::MantaOSC()
{
   OSCAddress = lo_address_new("localhost","8000");
   OSCServerThread = lo_server_thread_new("8001", ErrorHandler);
   /* the callbacks are static, so we need to pass the "this" pointer so that object methods
    * can be called from within the callbacks */
   lo_server_thread_add_method(OSCServerThread, "/Manta/LEDControl", "si", LEDControlHandler, this);
   lo_server_thread_add_method(OSCServerThread, "/Manta/LED/Pad", "si", LEDPadHandler, this);
   lo_server_thread_add_method(OSCServerThread, "/Manta/LED/Pad/Row", "sii", LEDRowHandler, this);
   lo_server_thread_add_method(OSCServerThread, "/Manta/LED/Pad/Column", "sii", LEDColumnHandler, this);
   lo_server_thread_add_method(OSCServerThread, "/Manta/LED/Pad/Frame", "ss", LEDFrameHandler, this);
   lo_server_thread_add_method(OSCServerThread, "/Manta/LED/Slider", "ii", LEDSliderHandler, this);
   lo_server_thread_add_method(OSCServerThread, "/Manta/LED/Button", "si", LEDButtonHandler, this);
   lo_server_thread_start(OSCServerThread);
}

MantaOSC::~MantaOSC()
{
   lo_server_thread_stop(OSCServerThread);
   lo_address_free(OSCAddress);
   lo_server_thread_free(OSCServerThread);
}

void MantaOSC::PadEvent(int id, int value)
{
   lo_send(OSCAddress, "/Manta/Pad", "ii", id, value);
}

void MantaOSC::SliderEvent(int id, int value)
{
   lo_send(OSCAddress, "/Manta/Slider", "ii", id, value);
}

void MantaOSC::ButtonEvent(int id, int value)
{
   lo_send(OSCAddress, "/Manta/Button", "ii", id, value);
}

int main(void)
{
   MantaOSC manta;
   do
   {
      try
      {
         manta.Connect();
      }
      catch(MantaNotFoundException &e)
      {
         std::cout << "Could not find an attached Manta. Retrying..." << std::endl;
         sleep(1);
      }
   } while(! manta.IsConnected());
   std::cout << "Manta Connected" << std::endl;
   try
   {
      while(1)
      {
         manta.HandleEvents();
      }
   }
   catch(MantaCommunicationException &e)
   {
      std::cout << "Communication with Manta interrupted, exiting..." << std::endl;
   }
   return 0;
}

int LEDControlHandler(const char *path, const char *types, lo_arg **argv, int argc,
      void *data, void *instancePointer)
{
   if(strcmp(&argv[0]->s, "PadAndButton") == 0)
   {
      static_cast<MantaOSC *>(instancePointer)->SetLEDControl(MantaOSC::PadAndButton, argv[1]->i);
   }
   else if(strcmp(&argv[0]->s, "Slider") == 0)
   {
      static_cast<MantaOSC *>(instancePointer)->SetLEDControl(MantaOSC::Slider, argv[1]->i);
   }
   else if(strcmp(&argv[0]->s, "Button") == 0)
   {
      static_cast<MantaOSC *>(instancePointer)->SetLEDControl(MantaOSC::Button, argv[1]->i);
   }
   else
   {
      return 1;
   }
   return 0;
}

int LEDPadHandler(const char *path, const char *types, lo_arg **argv, int argc,
      void *data, void *instancePointer)
{
   MantaOSC::LEDState state;
   try
   {
      state = getLEDStateFromString(&argv[0]->s);
      static_cast<MantaOSC *>(instancePointer)->SetPadLED(state, argv[1]->i);
      return 0;
   }
   catch(std::exception e)
   {
      /* catch any exception subclasses */
      std::cout << e.what() << std::endl;
      return 1;
   }
}

int LEDRowHandler(const char *path, const char *types, lo_arg **argv, int argc,
      void *data, void *instancePointer)
{
   MantaOSC::LEDState state;
   try
   {
      state = getLEDStateFromString(&argv[0]->s);
      static_cast<MantaOSC *>(instancePointer)->SetPadLEDRow(state, argv[1]->i,
            static_cast<uint8_t>(argv[2]->i));
      return 0;
   }
   catch(std::exception e)
   {
      /* catch any exception subclasses */
      std::cout << e.what() << std::endl;
      return 1;
   }
}

int LEDColumnHandler(const char *path, const char *types, lo_arg **argv, int argc,
      void *data, void *instancePointer)
{
   MantaOSC::LEDState state;
   try
   {
      state = getLEDStateFromString(&argv[0]->s);
      static_cast<MantaOSC *>(instancePointer)->SetPadLEDColumn(state, argv[1]->i,
            static_cast<uint8_t>(argv[2]->i));
      return 0;
   }
   catch(std::exception e)
   {
      /* catch any exception subclasses */
      std::cout << e.what() << std::endl;
      return 1;
   }
}

int LEDFrameHandler(const char *path, const char *types, lo_arg **argv, int argc,
      void *data, void *instancePointer)
{
   MantaOSC::LEDState state;
   try
   {
      state = getLEDStateFromString(&argv[0]->s);
      /* SetPadLEDFrame doesn't know how long the frame is, so we've got to check here */
      if(strlen(&argv[1]->s) != sizeof(MantaOSC::LEDFrame))
      {
         /* return unhandled, invalid frame length */
         return 1;
      }
      static_cast<MantaOSC *>(instancePointer)->SetPadLEDFrame(state, &argv[1]->c);
      return 0;
   }
   catch(std::exception e)
   {
      std::cout << e.what() << std::endl;
      return 1;
   }
}

int LEDSliderHandler(const char *path, const char *types, lo_arg **argv, int argc,
      void *data, void *instancePointer)
{
   MantaOSC::LEDState state;
   try
   {
      state = getLEDStateFromString(&argv[0]->s);
      static_cast<MantaOSC *>(instancePointer)->SetSliderLED(state, argv[1]->i,
            static_cast<uint8_t>(argv[2]->i));
      return 0;
   }
   catch(std::exception e)
   {
      std::cout << e.what() << std::endl;
      return 1;
   }
}

int LEDButtonHandler(const char *path, const char *types, lo_arg **argv, int argc,
      void *data, void *instancePointer)
{
   MantaOSC::LEDState state;
   try
   {
      state = getLEDStateFromString(&argv[0]->s);
      static_cast<MantaOSC *>(instancePointer)->SetButtonLED(state, argv[1]->i);
      return 0;
   }
   catch(std::exception e)
   {
      std::cout << e.what() << std::endl;
      return 1;
   }
}

void ErrorHandler(int num, const char *msg, const char *path)
{
   std::cout << "liblo server error " << num << " in path " << path << 
      ": " << msg << std::endl;
}

MantaOSC::LEDState getLEDStateFromString(const char *stateString)
{
   if(strcmp(stateString, "Red") == 0 ||
         strcmp(stateString, "red") == 0)
   {
      return MantaOSC::Red;
   }
   else if(strcmp(stateString, "Amber") == 0 ||
         strcmp(stateString, "amber") == 0)
   {
      return MantaOSC::Amber;
   }
   else if(strcmp(stateString, "Off") == 0 ||
         strcmp(stateString, "off") == 0)
   {
      return MantaOSC::Off;
   }
   else
   {
      throw std::invalid_argument("Invalid LEDState string");
   }
}
