#include <iostream>
#include "Application.h"

using namespace std;

void
Application::SetDeviceDriver(DeviceDriver* devicedriver)
{
    this->devicedriver = devicedriver;
}

vector<int>
Application::ReadAndPrint(long startAddr, long endAddr)
{
    vector<int> result = {};

    for (long Addr = startAddr; Addr <= endAddr; Addr++)
    {
        result.push_back(devicedriver->read(Addr));
    }

    return result;
}

void
Application::WriteAll(int value)
{
    devicedriver->write(0x00, value);
    devicedriver->write(0x01, value);
    devicedriver->write(0x02, value);
    devicedriver->write(0x03, value);
    devicedriver->write(0x04, value);
}
