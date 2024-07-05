

#include "DeviceDriver.h"

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    unsigned char nReadValue = m_hardware->read(address);
    for (int nRetry = 1; nRetry < READ_RETRY_COUNT; nRetry++)
    {
        if (nReadValue != m_hardware->read(address))
            throw ReadFailException();
    }

    return (int)nReadValue;
}

void DeviceDriver::write(long address, int data)
{
    unsigned char nPreReadValue = m_hardware->read(address);

    if (nPreReadValue != DATA_VALUE_ERASED)
        throw WriteFailException();

    m_hardware->write(address, (unsigned char)data);
}