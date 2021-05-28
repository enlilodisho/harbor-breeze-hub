#include "EventDispatcherForTests.h"
#include "FakeComponent.h"
#include "HarborBreezeHub/RFDataTransmittedEvent.h"
#include "HarborBreezeHub/RFTransmitter.h"

#include <gtest/gtest.h>
#include <vector>
#include <wiringPi.h>

constexpr int RF_TRANSMITTER_PIN = 21;

struct RFTransmitterTests : public ::testing::Test
{
    std::unique_ptr<RFTransmitter> rfTransmitter;
    std::unique_ptr<EventDispatcherForTests> eventDispatcherForTests;

    virtual void SetUp() override
    {
        wiringPiSetupGpio(); // initialize wiringPi using BCM pin numbers
        rfTransmitter = std::make_unique<RFTransmitter>("RFTransmitter", RF_TRANSMITTER_PIN);
        eventDispatcherForTests = std::make_unique<EventDispatcherForTests>();
        rfTransmitter->setEventDispatcher(eventDispatcherForTests.get());
    }

    virtual void TearDown() override
    {

    }
};

TEST_F(RFTransmitterTests, CorrectPinNumberTest)
{
    ASSERT_EQ(rfTransmitter->getPinNumber(), RF_TRANSMITTER_PIN);
}

TEST_F(RFTransmitterTests, TransmitDataTest)
{
    FakeComponent fakeComponent1("fakeComponent1");
    const std::string RFDATA_TRANSMITTED_EVENT_TYPE = "RFDataTransmittedEvent";
    eventDispatcherForTests->subscribe(&fakeComponent1, RFDATA_TRANSMITTED_EVENT_TYPE, rfTransmitter.get());

    //std::vector<unsigned int> lightToggleMessage { 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 400, 950, 400, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400, 10000, 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 400, 950, 400, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400, 10000, 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 400, 950, 400, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400, 10000, 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 400, 950, 400, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400, 10000, 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 400, 950, 400, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400, 10000, 400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 400, 950, 400, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400, 10000 };
    std::vector<unsigned int> dataToSend { 100, 200, 300 };
    rfTransmitter->transmit(dataToSend);
    rfTransmitter->doWork();

    auto events = eventDispatcherForTests->getEventsForComponent(&fakeComponent1);
    ASSERT_TRUE(events.size() > 0);
    bool found = false;
    for (auto& event : events)
    {
        if (event.first == rfTransmitter.get())
        {
            if (event.second->type() == RFDATA_TRANSMITTED_EVENT_TYPE)
            {
                std::shared_ptr<RFDataTransmittedEvent> rfDataEvent =
                        std::dynamic_pointer_cast<RFDataTransmittedEvent>(event.second);
                // Make sure rf data matches
                auto& data = rfDataEvent->getData();
                ASSERT_TRUE(dataToSend.size() == data.size());
                for (size_t i = 0; i < data.size(); i++)
                {
                    if (dataToSend[i] == data[i])
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
        if (found)
        {
            break;
        }
    }
    ASSERT_TRUE(found);
}