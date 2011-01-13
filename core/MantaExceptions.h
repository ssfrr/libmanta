#include <exception>

class MantaNotFoundException : public std::exception
{
};

class MantaCommunicationException : public std::exception
{
};
