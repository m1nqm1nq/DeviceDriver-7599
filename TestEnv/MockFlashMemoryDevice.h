#pragma once
#include "gmock/gmock.h"
#include "../Target/FlashMemoryDevice.h"

class MockFlashMemoryDevice : public FlashMemoryDevice
{
public:
    MOCK_METHOD(unsigned char, read, (long address), (override));
    MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};
