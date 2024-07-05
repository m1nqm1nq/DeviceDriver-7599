#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../Target/DeviceDriver.cpp"
#include "MockFlashMemoryDevice.h"

using namespace testing;

class DeviceDriverTestFixture : public testing::Test
{
protected:
public:
    NiceMock<MockFlashMemoryDevice> mockflash;
    DeviceDriver devicedriver{ &mockflash };

    static const int DATA_VALUE_ERASED = 0xFF;
    static const int DATA_VALUE_FOR_TEST = 0xA5;

    static const int DATA_ADDR_FOR_TEST = 0x1000;
};

TEST_F(DeviceDriverTestFixture, WriteAndReadNormal)
{
    // Arrange
    EXPECT_CALL(mockflash, read(_))
        .WillOnce(Return(DATA_VALUE_ERASED))
        .WillRepeatedly(Return(DATA_VALUE_FOR_TEST));

    // Act
    devicedriver.write(DATA_ADDR_FOR_TEST, DATA_VALUE_FOR_TEST);
    
    // Assert
    EXPECT_THAT(devicedriver.read(DATA_ADDR_FOR_TEST), Eq(DATA_VALUE_FOR_TEST));
}

TEST_F(DeviceDriverTestFixture, ReadErased)
{
    // Arrange
    EXPECT_CALL(mockflash, read(_))
        .WillRepeatedly(Return(DATA_VALUE_ERASED));

    // Act
    // Assert
    EXPECT_THAT(devicedriver.read(DATA_ADDR_FOR_TEST), Eq(DATA_VALUE_ERASED));
}

TEST_F(DeviceDriverTestFixture, Overwrite)
{
    // Arrange
    EXPECT_CALL(mockflash, read(_))
        .WillOnce(Return(DATA_VALUE_ERASED))
        .WillRepeatedly(Return(DATA_VALUE_FOR_TEST));

    // Act
    devicedriver.write(DATA_ADDR_FOR_TEST, DATA_VALUE_FOR_TEST);

    // Assert
    EXPECT_THROW(devicedriver.write(DATA_ADDR_FOR_TEST, DATA_VALUE_FOR_TEST), WriteFailException);
}

TEST_F(DeviceDriverTestFixture, ReadRetryFail)
{
    // Arrange
    EXPECT_CALL(mockflash, read(_))
        .WillOnce(Return(DATA_VALUE_ERASED))
        .WillOnce(Return(DATA_VALUE_FOR_TEST + 1))
        .WillRepeatedly(Return(DATA_VALUE_FOR_TEST));

    // Act
    devicedriver.write(DATA_ADDR_FOR_TEST, DATA_VALUE_FOR_TEST);

    // Assert
    EXPECT_THROW(devicedriver.read(DATA_ADDR_FOR_TEST), ReadFailException);
}