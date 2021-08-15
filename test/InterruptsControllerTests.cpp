#include "HarborBreezeHub/InterruptsController.h"

#include <gtest/gtest.h>
#include <wiringPi.h>

struct InterruptsControllerTests : public ::testing::Test
{
    virtual void SetUp() override
    {
        wiringPiSetupGpio(); // initialize wiringPi using BCM pin numbers
    }

    virtual void TearDown() override
    {

    }
};

TEST_F(InterruptsControllerTests, SingletonTest)
{
    InterruptsController& intController1 = InterruptsController::getInstance();
    InterruptsController& intController2 = InterruptsController::getInstance();
    ASSERT_EQ(&intController1, &intController2);
    ASSERT_NE(&intController1, nullptr);
}

TEST_F(InterruptsControllerTests, RegisterInterruptTest)
{
    auto empty_handler = [](const InterruptsController::clock::time_point& now) -> void {};
    // register pin
    Result r1 = InterruptsController::getInstance().registerInterrupt(1, INT_EDGE_BOTH, empty_handler);
    ASSERT_EQ(r1.success, true);
    // test registering duplicate causes failure
    Result r2 = InterruptsController::getInstance().registerInterrupt(1, INT_EDGE_BOTH, empty_handler);
    ASSERT_EQ(r2.success, false);
}