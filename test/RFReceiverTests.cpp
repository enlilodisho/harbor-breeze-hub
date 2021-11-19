#include "EventDispatcherForTests.h"
#include "HarborBreezeHub/RFReceiver.h"

#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
#include <gtest/gtest.h>
#include <stdexcept>

#define RF_RECEIVER_PIN                 16
#define RF_RECEIVER_PIN_GPIO_FILE       IRQ_TIMINGS_GPIO_FILE(16)
#define RF_RECEIVER_ALT_PIN             15
#define RF_RECEIVER_ALT_PIN_GPIO_FILE   IRQ_TIMINGS_GPIO_FILE(15)

struct RFReceiverTests : public ::testing::Test
{
    std::unique_ptr<RFReceiver> rfReceiver;
    std::unique_ptr<EventDispatcherForTests> eventDispatcherForTests;

    virtual void SetUp() override
    {
        rfReceiver = std::make_unique<RFReceiver>("RFReceiver", RF_RECEIVER_PIN);
        eventDispatcherForTests = std::make_unique<EventDispatcherForTests>();
        rfReceiver->setEventDispatcher(eventDispatcherForTests.get());
    }

    virtual void TearDown() override
    {

    }
};

TEST_F(RFReceiverTests, IRQTimingsDependencyCheck)
{
    ASSERT_TRUE(boost::filesystem::exists(IRQ_TIMINGS_PATH));
}

TEST_F(RFReceiverTests, GPIOPinRegisteredWithKernelModuleTest)
{
    ASSERT_TRUE(boost::filesystem::exists(RF_RECEIVER_PIN_GPIO_FILE));
}

TEST_F(RFReceiverTests, GPIOPinUnregisteredWithKernelModuleOnDestructorTest)
{
    auto rfReceiver2 = std::make_unique<RFReceiver>("RFReceiver2", RF_RECEIVER_ALT_PIN);
    ASSERT_TRUE(boost::filesystem::exists(RF_RECEIVER_ALT_PIN_GPIO_FILE));
    rfReceiver2.reset();
    ASSERT_FALSE(boost::filesystem::exists(RF_RECEIVER_ALT_PIN_GPIO_FILE));
}

TEST_F(RFReceiverTests, MultipleRFReceiversForPinThrowsErrorTest)
{
    ASSERT_THROW(std::make_unique<RFReceiver>("RFReceiver2", RF_RECEIVER_PIN),
            std::runtime_error);
}