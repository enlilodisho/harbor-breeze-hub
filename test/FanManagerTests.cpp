#include "HarborBreezeHub/FanManager.h"

#include <gtest/gtest.h>

struct FanManagerTests : public ::testing::Test
{
    std::unique_ptr<FanManager> fanManager;

    virtual void SetUp() override
    {
        fanManager = std::make_unique<FanManager>("FanManagerTest");
    }

    virtual void TearDown() override
    {

    }
};

TEST_F(FanManagerTests, CheckDefaultFanStateTest)
{
    // check defaults
    ASSERT_EQ(fanManager->getFanPower(), FanManager::DEFAULT_FAN_POWER);
    ASSERT_EQ(fanManager->getFanSpeed(), FanManager::DEFAULT_FAN_SPEED);
    ASSERT_EQ(fanManager->getFanLightPower(), FanManager::DEFAULT_FAN_LIGHT_POWER);
}

TEST_F(FanManagerTests, SetAndGetFanPowerTest)
{
    // set fan on
    fanManager->setFanPower(FanPower::FAN_ON);
    ASSERT_EQ(fanManager->getFanPower(), FanPower::FAN_ON);

    // set fan off
    fanManager->setFanPower(FanPower::FAN_OFF);
    ASSERT_EQ(fanManager->getFanPower(), FanPower::FAN_OFF);
    ASSERT_NE(fanManager->getFanPower(), FanPower::FAN_ON);
}

TEST_F(FanManagerTests, SetAndGetFanSpeedTest)
{
    // check setting fan speed
    const uint8_t fanSpeed = 4;
    fanManager->setFanSpeed(fanSpeed);
    ASSERT_EQ(fanManager->getFanSpeed(), fanSpeed);

    // make sure can't set fan speed to 0
    fanManager->setFanSpeed(0);
    ASSERT_NE(fanManager->getFanSpeed(), 0);
    ASSERT_EQ(fanManager->getFanSpeed(), fanSpeed);
}

TEST_F(FanManagerTests, SetAndGetFanLightPowerTest)
{
    // set fan light on
    fanManager->setFanLightPower(FanLightPower::FAN_LIGHT_ON);
    ASSERT_EQ(fanManager->getFanLightPower(), FanLightPower::FAN_LIGHT_ON);

    // set fan light off
    fanManager->setFanLightPower(FanLightPower::FAN_LIGHT_OFF);
    ASSERT_EQ(fanManager->getFanLightPower(), FanLightPower::FAN_LIGHT_OFF);
    ASSERT_NE(fanManager->getFanLightPower(), FanLightPower::FAN_LIGHT_ON);
}