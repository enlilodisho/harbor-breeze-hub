//
// Created by enlil on 4/7/21.
//

#include "FanManager.h"

FanManager::FanManager(std::string instanceName)
    : Component(instanceName)
{

}

FanManager::~FanManager()
{

}

void FanManager::setFanPower(FanPower newPowerSetting)
{
    fanPower = newPowerSetting;
}

FanPower FanManager::getFanPower() const
{
    return fanPower;
}

void FanManager::setFanSpeed(uint8_t newFanSpeed)
{
    if (newFanSpeed > 0)
    {
        fanSpeed = newFanSpeed;
    }
}

uint8_t FanManager::getFanSpeed() const
{
    return fanSpeed;
}

void FanManager::setFanLightPower(FanLightPower newPowerSetting)
{
    fanLightPower = newPowerSetting;
}

FanLightPower FanManager::getFanLightPower() const
{
    return fanLightPower;
}