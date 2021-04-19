//
// Created by enlil on 4/7/21.
//

#ifndef HARBORBREEZEHUB_FANMANAGER_H
#define HARBORBREEZEHUB_FANMANAGER_H

#include "ComponentEventSystem/Component.h"

#include <stdint.h>

enum FanPower
{
    FAN_OFF,
    FAN_ON
};

enum FanLightPower
{
    FAN_LIGHT_OFF,
    FAN_LIGHT_ON
};

class FanManager : public Component {
private:
    FanPower fanPower = DEFAULT_FAN_POWER;
    uint8_t fanSpeed = DEFAULT_FAN_SPEED;
    FanLightPower fanLightPower = DEFAULT_FAN_LIGHT_POWER;

public:
    static constexpr FanPower DEFAULT_FAN_POWER = FAN_OFF;
    static constexpr uint8_t DEFAULT_FAN_SPEED = 1;
    static constexpr FanLightPower DEFAULT_FAN_LIGHT_POWER = FAN_LIGHT_OFF;

    FanManager(std::string instanceName);
    ~FanManager() override;

    void setFanPower(FanPower newPowerSetting);
    FanPower getFanPower() const;
    void setFanSpeed(uint8_t newFanSpeed);
    uint8_t getFanSpeed() const;
    void setFanLightPower(FanLightPower newPowerSetting);
    FanLightPower getFanLightPower() const;

    ComponentType type() const override
    {
        return "FanManager";
    }
};


#endif //HARBORBREEZEHUB_FANMANAGER_H
