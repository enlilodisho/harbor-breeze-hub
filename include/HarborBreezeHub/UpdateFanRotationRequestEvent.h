//
// Created by enlil on 11/27/21.
//

#ifndef HARBORBREEZEHUB_UPDATEFANROTATIONREQUESTEVENT_H
#define HARBORBREEZEHUB_UPDATEFANROTATIONREQUESTEVENT_H

#include "core.h"
#include "FanState.h"

class UpdateFanRotationRequestEvent : public Event
{
private:
    const std::string fanName_;
    const Rotation newFanRotation_;

public:
    explicit UpdateFanRotationRequestEvent(const std::string& fanName, const Rotation& fanRotation)
            : fanName_(fanName), newFanRotation_(fanRotation)
    {
    }
    ~UpdateFanRotationRequestEvent() override = default;

    const std::string& getFanName() const
    {
        return fanName_;
    }

    const Rotation& getFanRotation() const
    {
        return newFanRotation_;
    }

    [[nodiscard]] EventType type() const override
    {
        return "UpdateFanRotationRequestEvent";
    }
};

#endif //HARBORBREEZEHUB_UPDATEFANROTATIONREQUESTEVENT_H
