//
// Created by enlil on 11/27/21.
//

#ifndef HARBORBREEZEHUB_UPDATEFANSPEEDREQUESTEVENT_H
#define HARBORBREEZEHUB_UPDATEFANSPEEDREQUESTEVENT_H

#include "core.h"
#include "FanState.h"

class UpdateFanSpeedRequestEvent : public Event
{
private:
    const std::string fanName_;
    const uint8_t newFanSpeed_;

public:
    explicit UpdateFanSpeedRequestEvent(const std::string& fanName, const uint8_t& fanSpeed)
            : fanName_(fanName), newFanSpeed_(fanSpeed)
    {
    }
    ~UpdateFanSpeedRequestEvent() override = default;

    const std::string& getFanName() const
    {
        return fanName_;
    }

    uint8_t getFanSpeed() const
    {
        return newFanSpeed_;
    }

    [[nodiscard]] EventType type() const override
    {
        return "UpdateFanSpeedRequestEvent";
    }
};

#endif //HARBORBREEZEHUB_UPDATEFANSPEEDREQUESTEVENT_H
