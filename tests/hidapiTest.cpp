#include <iostream>
#include <string>
#include <sstream>
#include "../core/extern/hidapi/hidapi/hidapi.h"

using namespace std;

typedef void (*Test)(void);

/* tests return true on success, false on failure */
void TestSingleConnect(void)
{
   hid_device *DeviceHandle;

   DeviceHandle = hid_open(0x2424, 0x2424, NULL);
   if(DeviceHandle)
   {
      cout << "Successfully Connected to One manta" << endl;
      hid_close(DeviceHandle);
   }
   else
   {
      cout << "Failed to Connect to One Manta";
   }
}

void TestDoubleConnect(void)
{
   hid_device *handle1, *handle2;
   wchar_t *serial1 = L"118";
   wchar_t *serial2 = L"58";
   handle1 = hid_open(0x2424, 0x2424, serial1);
   handle2 = hid_open(0x2424, 0x2424, serial2);

   if(handle1)
   {
      cout << "Connected to Manta 118, ";
      hid_close(handle1);
   }
   else
   {
      cout << "Failed Connecting to Manta 118, ";
   }
   if(handle2)
   {
      cout << "Connected to Manta 58" << endl;
      hid_close(handle2);
   }
   else
   {
      cout << "Failed Connecting to Manta 58" << endl;
   }
}

void TestCantReconnect(void)
{
   hid_device *handle1, *handle2;
   wchar_t *serial1 = L"118";
   handle1 = hid_open(0x2424, 0x2424, serial1);
   handle2 = hid_open(0x2424, 0x2424, serial1);

   if(handle1)
   {
      cout << "Connected to Manta 118, ";
      hid_close(handle1);
   }
   else
   {
      cout << "Failed Connecting to Manta 118, ";
   }
   if(handle2)
   {
      cout << "Connected to Manta 118 again witout disconnecting" << endl;
      hid_close(handle2);
   }
   else
   {
      cout << "Failed Connecting to Manta 118 without disconnecting" << endl;
   }
}

void TestFailsToConnectToMissingManta(void)
{
   hid_device *handle1;
   wchar_t *serial1 = L"110";
   handle1 = hid_open(0x2424, 0x2424, serial1);

   if(handle1)
   {
      cout << "Connected to Manta 110, which doesn't exist" << endl;
      hid_close(handle1);
   }
   else
   {
      cout << "Failed Connecting to Manta 110 because it isn't there" << endl;
   }
}

void runTest(Test test)
{
   test();
}

int main()
{
   Test TheTests[] = {
      TestSingleConnect,
      TestDoubleConnect,
      TestCantReconnect,
      TestFailsToConnectToMissingManta};

   for(int i = 0; i < (sizeof(TheTests)/sizeof(Test)); ++i)
   {
      runTest(TheTests[i]);
   }
}
