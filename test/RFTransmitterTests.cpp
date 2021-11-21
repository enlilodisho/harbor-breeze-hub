//
// Created by enlil on 11/19/21.
//
#include "ComponentMasterForTests.h"
#include "EventDispatcherForTests.h"
#include "FakeComponent.h"
#include "HarborBreezeHub/RFTransmitter.h"

#include <gtest/gtest.h>
#include <thread>
#include <wiringPi.h>

#define RF_TRANSMITTER_PIN                 21

struct RFTransmitterTests : public ::testing::Test
{
    RFTransmitter* rfTransmitter;
    EventDispatcherForTests* eventDispatcherForTests;
    FakeComponent* fakeComponent;
    std::unique_ptr<ComponentMasterForTests> componentMasterForTests;

    void SetUp() override
    {
        wiringPiSetupGpio();
        // Create ComponentMaster for Tests
        componentMasterForTests = std::make_unique<ComponentMasterForTests>();
        auto eventDispatcher_unique_ptr = std::make_unique<EventDispatcherForTests>();
        eventDispatcherForTests = eventDispatcher_unique_ptr.get();
        componentMasterForTests->setEventDispatcher(std::move(eventDispatcher_unique_ptr));

        // Create fake component
        auto fakeComponent_unique_ptr = std::make_unique<FakeComponent>("FakeComponent");
        fakeComponent = fakeComponent_unique_ptr.get();
        componentMasterForTests->addComponent(std::move(fakeComponent_unique_ptr));

        // Create a RFTransmitter component
        auto rfTransmitter_unique_ptr = std::make_unique<RFTransmitter>("RFTransmitter", RF_TRANSMITTER_PIN);
        rfTransmitter = rfTransmitter_unique_ptr.get();
        componentMasterForTests->addComponent(std::move(rfTransmitter_unique_ptr));
        componentMasterForTests->getEventDispatcher().subscribe(rfTransmitter, "TransmitRFDataRequestEvent", fakeComponent);

        // Register fake component as receiver of transmitter events
        componentMasterForTests->getEventDispatcher().subscribe(fakeComponent, "TransmitRFDataBeginEvent", rfTransmitter);
        componentMasterForTests->getEventDispatcher().subscribe(fakeComponent, "TransmitRFDataEndEvent", rfTransmitter);

        // Start components
        componentMasterForTests->runOnStartOnAllComponents();
    }

    void TearDown() override
    {
        componentMasterForTests->runOnStopOnAllComponents();
    }
};

TEST_F(RFTransmitterTests, RequestBeginAndEndTransmitEventsTest)
{
    componentMasterForTests->update();
    ASSERT_TRUE(eventDispatcherForTests->getEventsForComponent(fakeComponent).empty());
    ASSERT_TRUE(eventDispatcherForTests->getEventsForComponent(rfTransmitter).empty());

    std::shared_ptr<std::vector<unsigned int>> timings = std::make_shared<std::vector<unsigned int>>();
    timings->push_back(0);
    eventDispatcherForTests->post(fakeComponent, std::make_unique<TransmitRFDataRequestEvent>(std::move(timings), 0, 1));
    auto events = eventDispatcherForTests->getEventsForComponent(rfTransmitter);
    ASSERT_EQ(events.size(), 1);
    ASSERT_EQ(events[0].second->type(), "TransmitRFDataRequestEvent");

    componentMasterForTests->update();
    events = eventDispatcherForTests->getEventsForComponent(fakeComponent);
    ASSERT_EQ(events.size(), 1);
    ASSERT_EQ(events[0].second->type(), "TransmitRFDataBeginEvent");

    componentMasterForTests->update();
    events = eventDispatcherForTests->getEventsForComponent(fakeComponent);
    ASSERT_EQ(events.size(), 1);
    ASSERT_EQ(events[0].second->type(), "TransmitRFDataEndEvent");

    componentMasterForTests->update();
    ASSERT_TRUE(eventDispatcherForTests->getEventsForComponent(fakeComponent).empty());
    ASSERT_TRUE(eventDispatcherForTests->getEventsForComponent(rfTransmitter).empty());
}

// transmission for fan light power:
//     std::vector<unsigned int> t = {400, 500, 850, 950, 400, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 500, 850, 500, 850, 950, 400, 950, 400, 950, 400, 950, 400, 500, 850, 500, 850, 950, 400, 500, 850, 950, 400, 500, 850, 950, 400};