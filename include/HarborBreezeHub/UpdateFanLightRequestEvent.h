//
// Created by enlil on 11/27/21.
//

#ifndef HARBORBREEZEHUB_UPDATEFANLIGHTREQUESTEVENT_H
#define HARBORBREEZEHUB_UPDATEFANLIGHTREQUESTEVENT_H

#include "core.h"
#include "FanState.h"

class UpdateFanLightRequestEvent : public Event
{
private:
    const std::string fanName_;
    const Power newLightPower_;

public:
    explicit UpdateFanLightRequestEvent(const std::string& fanName, const Power& lightPower)
            : fanName_(fanName), newLightPower_(lightPower)
    {
    }
    ~UpdateFanLightRequestEvent() override = default;

    [[nodiscard]] const std::string& getFanName() const
    {
        return fanName_;
    }

    [[nodiscard]] const Power& getLightPower() const
    {
        return newLightPower_;
    }

    [[nodiscard]] EventType type() const override
    {
        return "UpdateFanLightRequestEvent";
    }
};

#endif //HARBORBREEZEHUB_UPDATEFANLIGHTREQUESTEVENT_H
