#pragma once
#include <vector>
#include "DeviceDriver.h"

using std::vector;

class Application
{
public:
    void SetDeviceDriver(DeviceDriver* devicedriver);
    vector<int> ReadAndPrint(long startAddr, long endAddr);
    void WriteAll(int value);

private:
    DeviceDriver* devicedriver;
};