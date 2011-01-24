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

class MantaCommunicationException : public std::runtime_error
{
   public:
      MantaCommunicationException() :
            runtime_error("Communication with Manta interrupted")
         {
         }
};
