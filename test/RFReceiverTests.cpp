#include "HarborBreezeHub/RFReceiver.h"

#include <gtest/gtest.h>
#include <wiringPi.h>

constexpr int RF_RECEIVER_PIN = 16;

struct RFReceiverTests : public ::testing::Test
{
    std::unique_ptr<RFReceiver> rfReceiver;

    virtual void SetUp() override
    {
        wiringPiSetupGpio(); // initialize wiringPi using BCM pin numbers
        rfReceiver = std::make_unique<RFReceiver>("RFReceiver", RF_RECEIVER_PIN);
    }

    virtual void TearDown() override
    {

    }
};

TEST_F(RFReceiverTests, CorrectPinNumbersTest)
{
    ASSERT_EQ(rfReceiver->getPinNumber(), RF_RECEIVER_PIN);
}