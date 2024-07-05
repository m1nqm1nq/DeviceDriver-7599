#pragma once
#include "FlashMemoryDevice.h"
#include "FailException.h"

class DeviceDriver
{
public:
    DeviceDriver() {}
    DeviceDriver(FlashMemoryDevice* hardware);
    virtual int read(long address);
    virtual void write(long address, int data);

protected:
    FlashMemoryDevice* m_hardware;

private:
    static const int READ_RETRY_COUNT = 5;
    static const unsigned char DATA_VALUE_ERASED = 0xFF;
};