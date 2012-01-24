#include "MantaMulti.h"
#include <algorithm>
#include <cstdarg>
#include <cstdio>

MantaMulti::MantaMulti(MantaClient *client) :
   ReferenceCount(0)
{
   AttachClient(client);
}

void MantaMulti::AttachClient(MantaClient *client)
{
   if(NULL != client)
   {
      ClientList.push_back(client);
      ++ReferenceCount;
   }
}

void MantaMulti::DetachClient(MantaClient *client)
{
   list<MantaClient *>::iterator foundIter;
   foundIter = find(ClientList.begin(), ClientList.end(), client);
   if(ClientList.end() != foundIter)
   {
      ClientList.erase(foundIter);
      --ReferenceCount;
   }
}

int MantaMulti::GetReferenceCount()
{
   return ReferenceCount;
}

void MantaMulti::PadEvent(int row, int column, int id, int value)
{
   for(list<MantaClient *>::iterator i = ClientList.begin();
         i != ClientList.end(); ++i)
   {
      (*i)->PadEvent(row, column, id, value);
   }
}

void MantaMulti::SliderEvent(int id, int value)
{
   for(list<MantaClient *>::iterator i = ClientList.begin();
         i != ClientList.end(); ++i)
   {
      (*i)->SliderEvent(id, value);
   }
}

void MantaMulti::ButtonEvent(int id, int value)
{
   for(list<MantaClient *>::iterator i = ClientList.begin();
         i != ClientList.end(); ++i)
   {
      (*i)->ButtonEvent(id, value);
   }
}

void MantaMulti::PadVelocityEvent(int row, int column, int id, int velocity)
{
   for(list<MantaClient *>::iterator i = ClientList.begin();
         i != ClientList.end(); ++i)
   {
      (*i)->PadVelocityEvent(row, column, id, velocity);
   }
}

void MantaMulti::ButtonVelocityEvent(int id, int velocity)
{
   for(list<MantaClient *>::iterator i = ClientList.begin();
         i != ClientList.end(); ++i)
   {
      (*i)->ButtonVelocityEvent(id, velocity);
   }
}

void MantaMulti::FrameEvent(uint8_t *frame)
{
   for(list<MantaClient *>::iterator i = ClientList.begin();
         i != ClientList.end(); ++i)
   {
      (*i)->FrameEvent(frame);
   }
}

/*
void MantaMulti::DebugPrint(const char *fmt, ...)
{
   if(!ClientList.empty())
   {
      va_list args;
      char string[256];
      va_start(args, fmt);
      vsprintf(string, fmt, args);
      va_end (args);
      ClientList.front()->DebugPrint(string);
   }
}
*/

