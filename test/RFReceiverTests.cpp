#include "EventDispatcherForTests.h"
#include "FakeComponent.h"
#include "HarborBreezeHub/RFDataEvent.h"
#include "HarborBreezeHub/RFReceiver.h"

#include <gtest/gtest.h>
#include <wiringPi.h>

constexpr int RF_RECEIVER_PIN = 16;

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

TEST_F(RFReceiverTests, ListenForDataTest)
{
    std::vector<unsigned int> sampleData { 200, 350, 100, 410 };
    ASSERT_TRUE(rfReceiver->listenForData("SAMPLE_DATA", sampleData).success);

    EventType rfDataEventType = "RFDataEvent";
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
                std::shared_ptr<RFDataEvent> rfDataEvent =
                        std::dynamic_pointer_cast<RFDataEvent>(event.second);
                if (rfDataEvent->getDataLabel() == "SAMPLE_DATA")
                {
                    // Make sure rf data matches
                    auto& data = rfDataEvent->getData();
                    ASSERT_TRUE(sampleData.size() == data.size());
                    for (size_t i = 0; i < data.size(); i++)
                    {
                        if (sampleData[i] == data[i])
                        {
                            if (i+1 == data.size())
                            {
                                found = true;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
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