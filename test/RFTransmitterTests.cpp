#include "HarborBreezeHub/RFTransmitter.h"

#include <gtest/gtest.h>
#include <wiringPi.h>

constexpr int RF_TRANSMITTER_PIN = 21;

struct RFTransmitterTests : public ::testing::Test
{
    std::unique_ptr<RFTransmitter> rfTransmitter;

    virtual void SetUp() override
    {
        wiringPiSetupGpio(); // initialize wiringPi using BCM pin numbers
        rfTransmitter = std::make_unique<RFTransmitter>("RFTransmitter", RF_TRANSMITTER_PIN);
    }

    virtual void TearDown() override
    {

    }
};

TEST_F(RFTransmitterTests, CorrectPinNumberTest)
{
    ASSERT_EQ(rfTransmitter->getPinNumber(), RF_TRANSMITTER_PIN);
}