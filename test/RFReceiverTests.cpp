#include "EventDispatcherForTests.h"
#include "HarborBreezeHub/RFReceiver.h"

#include <gtest/gtest.h>
#include <stdexcept>
#include <wiringPi.h>

constexpr int RF_RECEIVER_PIN = 16;
constexpr int RF_RECEIVER_ALT_PIN = 15;

struct RFReceiverTests : public ::testing::Test
{
    std::unique_ptr<RFReceiver> rfReceiver;
    std::unique_ptr<EventDispatcherForTests> eventDispatcherForTests;

    virtual void SetUp() override
    {
        wiringPiSetupGpio(); // initialize wiringPi using BCM pin numbers
        rfReceiver = std::make_unique<RFReceiver>("RFReceiver", RF_RECEIVER_PIN);
        eventDispatcherForTests = std::make_unique<EventDispatcherForTests>();
        rfReceiver->setEventDispatcher(eventDispatcherForTests.get());
    }

    virtual void TearDown() override
    {

    }
};

TEST_F(RFReceiverTests, MultipleRFReceiversForPinThrowsErrorTest)
{
    ASSERT_THROW(std::make_unique<RFReceiver>("RFReceiver2", RF_RECEIVER_PIN),
            std::logic_error);
    ASSERT_NO_THROW(std::make_unique<RFReceiver>("RFReceiver2", RF_RECEIVER_ALT_PIN));
}