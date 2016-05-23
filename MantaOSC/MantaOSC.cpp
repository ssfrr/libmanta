#include "../core/Manta.h"
#include "../core/MantaExceptions.h"
#include <lo/lo.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <stdexcept>

class MantaOSC : public Manta
{
   public:
      MantaOSC(int serverPort, int clientPort);
      ~MantaOSC();
   private:
      virtual void PadEvent(int row, int column, int id, int value);
      virtual void SliderEvent(int id, int value);
      virtual void ButtonEvent(int id, int value);
      virtual void PadVelocityEvent(int row, int column, int id, int value);
      virtual void ButtonVelocityEvent(int id, int value);
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

MantaOSC::MantaOSC(int serverPort, int clientPort)
{
   char serverPortString[16];
   char clientPortString[16];
   sprintf(serverPortString, "%d", serverPort);
   sprintf(clientPortString, "%d", clientPort);
   OSCServerThread = lo_server_thread_new(serverPortString, ErrorHandler);
   OSCAddress = lo_address_new("127.0.0.1", clientPortString);
   /* the callbacks are static, so we need to pass the "this" pointer so that object methods
    * can be called from within the callbacks */
   lo_server_thread_add_method(OSCServerThread, "/manta/ledcontrol", "si", LEDControlHandler, this);
   lo_server_thread_add_method(OSCServerThread, "/manta/led/pad", "si", LEDPadHandler, this);
   lo_server_thread_add_method(OSCServerThread, "/manta/led/pad/row", "sii", LEDRowHandler, this);
   lo_server_thread_add_method(OSCServerThread, "/manta/led/pad/column", "sii", LEDColumnHandler, this);
   lo_server_thread_add_method(OSCServerThread, "/manta/led/pad/frame", "ss", LEDFrameHandler, this);
   lo_server_thread_add_method(OSCServerThread, "/manta/led/slider", "sii", LEDSliderHandler, this);
   lo_server_thread_add_method(OSCServerThread, "/manta/led/button", "si", LEDButtonHandler, this);
   lo_server_thread_start(OSCServerThread);
}

MantaOSC::~MantaOSC()
{
   lo_server_thread_stop(OSCServerThread);
   lo_address_free(OSCAddress);
   lo_server_thread_free(OSCServerThread);
}

void MantaOSC::PadEvent(int row, int column, int id, int value)
{
   int retVal;
   retVal = lo_send(OSCAddress, "/manta/continuous/pad", "ii", id, value);
   if(retVal == -1)
   {
       printf("Error Sending: \"%s\"\n", lo_address_errstr(OSCAddress));
   }
}

void MantaOSC::SliderEvent(int id, int value)
{
   lo_send(OSCAddress, "/manta/continuous/slider", "ii", id, value);
}

void MantaOSC::ButtonEvent(int id, int value)
{
   lo_send(OSCAddress, "/manta/continuous/button", "ii", id, value);
}

void MantaOSC::PadVelocityEvent(int row, int column, int id, int value)
{
   lo_send(OSCAddress, "/manta/velocity/pad", "ii", id, value);
}

void MantaOSC::ButtonVelocityEvent(int id, int value)
{
   lo_send(OSCAddress, "/manta/velocity/button", "ii", id, value);
}

#define DEFAULT_SERVER_PORT 31417
#define DEFAULT_CLIENT_PORT 31416

int main(int argc, char *argv[])
{
   int serial;
   int serverPort, clientPort;
   /* current usage is MantaOSC [SERIAL [SERVERPORT [CLIENTPORT]]]
    * if you want to supply a port you have to supply a serial number. At some
    * point this should do proper arg handling.
    */
   if(argc < 2)
   {
       serial = 0;
       serverPort = DEFAULT_SERVER_PORT;
       clientPort = DEFAULT_CLIENT_PORT;
   }
   else
   {
       serial = atoi(argv[1]);
       if(argc < 3)
       {
           serverPort = DEFAULT_SERVER_PORT;
           clientPort = DEFAULT_CLIENT_PORT;
       }
       else
       {
           serverPort = atoi(argv[2]);
           if(argc < 4)
           {
               clientPort = DEFAULT_CLIENT_PORT;
           }
           else
           {
               clientPort = atoi(argv[3]);
           }
       }
   }
   MantaOSC manta(serverPort, clientPort);
   do
   {
      try
      {
         manta.Connect(serial);
      }
      catch(MantaNotFoundException &e)
      {
         if(serial == 0) {
            std::cout << "Could not find an attached Manta. Retrying..." << std::endl;
         }
         else {
            std::cout << "Could not find a Manta with serial " << serial <<". Retrying..." << std::endl;
         }
         sleep(1);
      }
   } while(! manta.IsConnected());
   std::cout << "Connected to Manta" << std::endl;
   std::cout << "  Serial Number: " << manta.GetSerialNumber() << std::endl;
   std::cout << "  Firmware Version: " << manta.GetHardwareVersion() << std::endl;
   std::cout << "Listening for OSC Messages on port " << serverPort << std::endl;
   std::cout << "Sending OSC Messages to port " << clientPort << std::endl;
   try
   {
      while(1)
      {
         manta.HandleEvents();
         usleep(2000);
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
   if(strcmp(&argv[0]->s, "padandbutton") == 0)
   {
      static_cast<MantaOSC *>(instancePointer)->SetLEDControl(MantaOSC::PadAndButton, argv[1]->i);
   }
   else if(strcmp(&argv[0]->s, "slider") == 0)
   {
      static_cast<MantaOSC *>(instancePointer)->SetLEDControl(MantaOSC::Slider, argv[1]->i);
   }
   else if(strcmp(&argv[0]->s, "button") == 0)
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
