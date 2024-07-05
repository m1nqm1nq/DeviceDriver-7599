#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../Target/DeviceDriver.cpp"
#include "MockFlashMemoryDevice.h"

#include "../Target/Application.cpp"
#include "MockDeviceDriver.h"

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


class ApplicationTestFixture : public testing::Test
{
protected:
    void SetUp() override
    {
        app.SetDeviceDriver(&mockdevicedriver);
    }
public:
    Application app;
    MockDeviceDriver mockdevicedriver;

    static const int DATA_VALUE_FOR_TEST = 0xA5;
    static const int DATA_VALUE_ERASED = 0xFF;
};

TEST_F(ApplicationTestFixture, WriteAll0xA5)
{
    // Arrange
    EXPECT_CALL(mockdevicedriver, write(_, DATA_VALUE_FOR_TEST))
        .Times(5);

    // Act
    app.WriteAll(DATA_VALUE_FOR_TEST);

    // Assert
}

TEST_F(ApplicationTestFixture, ReadAndPrintAll0xA5)
{
    vector<int> actual = { };
    vector<int> expect =
    {
        DATA_VALUE_FOR_TEST,
        DATA_VALUE_FOR_TEST,
        DATA_VALUE_FOR_TEST,
        DATA_VALUE_FOR_TEST,
        DATA_VALUE_FOR_TEST
    };

    // Arrange
    EXPECT_CALL(mockdevicedriver, write(_, DATA_VALUE_FOR_TEST))
        .Times(5);
    EXPECT_CALL(mockdevicedriver, read(0x00))
        .Times(1)
        .WillRepeatedly(Return(DATA_VALUE_FOR_TEST));
    EXPECT_CALL(mockdevicedriver, read(0x01))
        .Times(1)
        .WillRepeatedly(Return(DATA_VALUE_FOR_TEST));
    EXPECT_CALL(mockdevicedriver, read(0x02))
        .Times(1)
        .WillRepeatedly(Return(DATA_VALUE_FOR_TEST));
    EXPECT_CALL(mockdevicedriver, read(0x03))
        .Times(1)
        .WillRepeatedly(Return(DATA_VALUE_FOR_TEST));
    EXPECT_CALL(mockdevicedriver, read(0x04))
        .Times(1)
        .WillRepeatedly(Return(DATA_VALUE_FOR_TEST));

    // Act
    app.WriteAll(DATA_VALUE_FOR_TEST);
    actual = app.ReadAndPrint(0x00, 0x04);

    // Assert
    EXPECT_THAT(actual, Eq(expect));
}


TEST_F(ApplicationTestFixture, Two0xA5sAndThree0xFFs)
{
    vector<int> actual = { };
    vector<int> expect =
    {
        DATA_VALUE_FOR_TEST,
        DATA_VALUE_FOR_TEST,
        DATA_VALUE_ERASED,
        DATA_VALUE_ERASED,
        DATA_VALUE_ERASED
    };

    // Arrange
    EXPECT_CALL(mockdevicedriver, write(_, DATA_VALUE_FOR_TEST))
        .Times(5);
    EXPECT_CALL(mockdevicedriver, read(0x03))
        .Times(1)
        .WillRepeatedly(Return(DATA_VALUE_FOR_TEST));
    EXPECT_CALL(mockdevicedriver, read(0x04))
        .Times(1)
        .WillRepeatedly(Return(DATA_VALUE_FOR_TEST));
    EXPECT_CALL(mockdevicedriver, read(0x05))
        .Times(1)
        .WillRepeatedly(Return(DATA_VALUE_ERASED));
    EXPECT_CALL(mockdevicedriver, read(0x06))
        .Times(1)
        .WillRepeatedly(Return(DATA_VALUE_ERASED));
    EXPECT_CALL(mockdevicedriver, read(0x07))
        .Times(1)
        .WillRepeatedly(Return(DATA_VALUE_ERASED));

    // Act
    app.WriteAll(DATA_VALUE_FOR_TEST);
    actual = app.ReadAndPrint(0x03, 0x07);

    // Assert
    EXPECT_THAT(actual, Eq(expect));
}