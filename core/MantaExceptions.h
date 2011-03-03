#ifndef _MANTAEXCEPTIONS_H
#define _MANTAEXCEPTIONS_H

#include <stdexcept>

class LibusbInitException : public std::runtime_error
{
   public:
      LibusbInitException() :
            runtime_error("Error initializing libusb")
         {
         }
};

class MantaNotConnectedException : public std::runtime_error
{
   public:
      MantaNotConnectedException() :
            runtime_error("Attempted to access the Manta without connecting")
         {
         }
};

class MantaNotFoundException : public std::runtime_error
{
   public:
      MantaNotFoundException() :
            runtime_error("Could not find an attached Manta")
         {
         }
};

class MantaOpenException : public std::runtime_error
{
   public:
      MantaOpenException() :
            runtime_error("Could not connect to attached Manta")
         {
         }
};

class MantaCommunicationException : public std::runtime_error
{
   public:
      MantaCommunicationException() :
            runtime_error("Communication with Manta interrupted")
         {
         }
};

#endif // _MANTAEXCEPTIONS_H
