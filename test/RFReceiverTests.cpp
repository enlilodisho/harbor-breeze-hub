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

TEST_F(RFReceiverTests, AddThenRemoveDataToListenForTest)
{
    ASSERT_FALSE(rfReceiver->stopListeningForData("SAMPLE_DATA"));

    std::vector<unsigned int> sampleData { 100, 100 };
    ASSERT_TRUE(rfReceiver->listenForData("SAMPLE_DATA", sampleData).success);
    ASSERT_FALSE(rfReceiver->listenForData("SAMPLE_DATA", sampleData).success);
    ASSERT_TRUE(rfReceiver->stopListeningForData("SAMPLE_DATA"));
}

bool checkIfRFReceivedEventMatchesData(const RFDataReceivedEvent& dataReceivedEvent, const std::vector<unsigned int>& data)
{
    // Make sure rf data matches
    auto& dataFromEvent = dataReceivedEvent.getData();
    if (data.size() != dataFromEvent.size())
    {
        return false;
    }

    for (size_t i = 0; i < data.size(); i++)
    {
        if (dataFromEvent[i] == data[i])
        {
            if (i+1 == data.size())
            {
                return true;
            }
        }
        else
        {
            break;
        }
    }
    return false;
}

TEST_F(RFReceiverTests, ListenForDataTest)
{
    std::vector<unsigned int> sampleData { 200, 350, 100, 410 };
    ASSERT_TRUE(rfReceiver->listenForData("SAMPLE_DATA", sampleData).success);

    EventType rfDataEventType = "RFDataReceivedEvent";
    FakeComponent fakeComponent1("fakeComponent1");
    eventDispatcherForTests->subscribe(&fakeComponent1, rfDataEventType, rfReceiver.get());

    auto events = eventDispatcherForTests->getEventsForComponent(&fakeComponent1);
    ASSERT_TRUE(events.size() == 0);

    std::vector<unsigned int> sampleRecvData { 205, 343, 103, 395 }; // recv slightly different values
    rfReceiver->receive(sampleRecvData);
    rfReceiver->doWork();

    events = eventDispatcherForTests->getEventsForComponent(&fakeComponent1);
    ASSERT_TRUE(events.size() > 0);
    bool found = false;
    for (auto& event : events)
    {
        if (event.first == rfReceiver.get())
        {
            if (event.second->type() == rfDataEventType)
            {
                std::shared_ptr<RFDataReceivedEvent> rfDataEvent =
                        std::dynamic_pointer_cast<RFDataReceivedEvent>(event.second);
                if (rfDataEvent->getDataLabel() == "SAMPLE_DATA")
                {
                    found = checkIfRFReceivedEventMatchesData(*rfDataEvent.get(), sampleData);
                }
            }
        }
        if (found)
        {
            break;
        }
    }
    ASSERT_TRUE(found);
}

TEST_F(RFReceiverTests, ReceiveDataTransmittedFromTransmitterTest)
{
    std::vector<unsigned int> dataToTransmit { 700, 800, 100, 300, 100, 200, 700, 100, 200, 300, 400, 700, 800, 100, 300, 100, 200, 700, 100, 200, 300, 400, 700, 800, 100, 300, 100, 200, 700, 100, 200, 300, 400, 700, 800, 100, 300, 100, 200, 700, 100, 200, 300, 400, 700, 800, 100, 300, 100, 200, 700, 100, 200, 300, 400, 700, 800, 100, 300, 100, 200, 700, 100, 200, 300, 400 };
    std::vector<unsigned int> dataToReceive { 700, 800, 100, 300, 100, 200, 700, 100, 200, 300, 400 };
    ASSERT_TRUE(rfReceiver->listenForData("SAMPLE_DATA2", dataToReceive).success);
    FakeComponent fakeComponent1("fakeComponent1");
    eventDispatcherForTests->subscribe(&fakeComponent1, "RFDataReceivedEvent", rfReceiver.get());

    // Create transmitter & transmit data
    RFTransmitter rfTransmitter("RFTransmitter", RF_TRANSMITTER_PIN);
    rfTransmitter.setEventDispatcher(eventDispatcherForTests.get());
    rfTransmitter.transmit(dataToTransmit);
    rfTransmitter.doWork();

    unsigned int i = 0;
    while (i++ < 1000000);

    rfReceiver->doWork();

    auto events = eventDispatcherForTests->getEventsForComponent(&fakeComponent1);
    ASSERT_TRUE(events.size() > 0);
    bool found = false;
    for (auto& event : events)
    {
        if (event.first == rfReceiver.get())
        {
            if (event.second->type() == "RFDataReceivedEvent")
            {
                std::shared_ptr<RFDataReceivedEvent> rfDataEvent =
                        std::dynamic_pointer_cast<RFDataReceivedEvent>(event.second);
                if (rfDataEvent->getDataLabel() == "SAMPLE_DATA2")
                {
                    found = checkIfRFReceivedEventMatchesData(*rfDataEvent.get(), dataToReceive);
                }
            }
        }
        if (found)
        {
            break;
        }
    }
    ASSERT_TRUE(found);
}