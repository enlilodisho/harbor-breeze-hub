//
// Created by enlil on 11/27/21.
//

#ifndef HARBORBREEZEHUB_UPDATEFANPOWERREQUESTEVENT_H
#define HARBORBREEZEHUB_UPDATEFANPOWERREQUESTEVENT_H

#include "core.h"
#include "FanState.h"

class UpdateFanPowerRequestEvent : public Event
{
private:
    const std::string fanName_;
    const Power newFanPower_;

public:
    explicit UpdateFanPowerRequestEvent(const std::string& fanName, const Power& fanPower)
            : fanName_(fanName), newFanPower_(fanPower)
    {
    }
    ~UpdateFanPowerRequestEvent() override = default;

    [[nodiscard]] const std::string& getFanName() const
    {
        return fanName_;
    }

    [[nodiscard]] const Power& getFanPower() const
    {
        return newFanPower_;
    }

    [[nodiscard]] EventType type() const override
    {
        return "UpdateFanPowerRequestEvent";
    }
};

#endif //HARBORBREEZEHUB_UPDATEFANPOWERREQUESTEVENT_H
