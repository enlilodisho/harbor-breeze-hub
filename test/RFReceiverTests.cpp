#include "EventDispatcherForTests.h"
#include "FakeComponent.h"
#include "HarborBreezeHub/RFDataReceivedEvent.h"
#include "HarborBreezeHub/RFDataTransmittedEvent.h"
#include "HarborBreezeHub/RFReceiver.h"
#include "HarborBreezeHub/RFTransmitter.h"

#include <gtest/gtest.h>
#include <vector>
#include <wiringPi.h>
#include <iostream>

constexpr int RF_RECEIVER_PIN = 16;
constexpr int RF_TRANSMITTER_PIN = 21;

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

TEST_F(RFReceiverTests, CorrectPinNumbersTest)
{
    ASSERT_EQ(rfReceiver->getPinNumber(), RF_RECEIVER_PIN);
}

TEST_F(RFReceiverTests, ReceiveDataTransmittedFromTransmitterTest)
{
    /*std::vector<unsigned int> dataToTransmit { 700, 800, 100, 300, 100, 200, 700, 100, 200, 300, 400, 10000, 700, 800, 100, 300, 100, 200, 700, 100, 200, 300, 400, 10000, 700, 800, 100, 300, 100, 200, 700, 100, 200, 300, 400, 10000, 700, 800, 100, 300, 100, 200, 700, 100, 200, 300, 400, 10000, 700, 800, 100, 300, 100, 200, 700, 100, 200, 300, 400, 10000, 700, 800, 100, 300, 100, 200, 700, 100, 200, 300, 400, 10000 };
    std::vector<unsigned int> dataToReceive { 700, 800, 100, 300, 100, 200, 700, 100, 200, 300, 400, 10000 };*/
    std::vector<unsigned int> dataToTransmit { 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 400, 950, 400, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400, 10000, 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 400, 950, 400, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400, 10000, 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 400, 950, 400, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400, 10000, 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 400, 950, 400, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400, 10000, 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 400, 950, 400, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400, 10000, 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 400, 950, 400, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400, 10000 };

    // Have FakeComponent subscribe to events from this RFReceiver
    FakeComponent fakeComponent1("fakeComponent1");
    eventDispatcherForTests->subscribe(&fakeComponent1, "RFDataReceivedEvent", rfReceiver.get());

    // Create transmitter & transmit data
    RFTransmitter rfTransmitter("RFTransmitter", RF_TRANSMITTER_PIN);
    rfTransmitter.setEventDispatcher(eventDispatcherForTests.get());
    rfTransmitter.transmit(dataToTransmit);
    rfTransmitter.doWork();

    size_t i = 0;
    while (i++ < 1000000)
    {
        ASSERT_TRUE((i < 1000000));
    }

    // Get events from RFReceiver
    auto events = eventDispatcherForTests->getEventsForComponent(&fakeComponent1);

    ASSERT_TRUE(events.size() > 0);
    // TODO confirm received event(s) is for the transmitted data
}