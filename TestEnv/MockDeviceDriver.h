#pragma once
#include "gmock/gmock.h"
#include "../Target/DeviceDriver.h"

class MockDeviceDriver : public DeviceDriver
{
public:
    MOCK_METHOD(int, read, (long address), (override));
    MOCK_METHOD(void, write, (long address, int data), (override));
};
