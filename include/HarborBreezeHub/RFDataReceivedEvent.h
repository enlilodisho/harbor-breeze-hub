//
// Created by enlil on 5/27/21.
//

#ifndef HARBORBREEZEHUB_RFDATARECEIVEDEVENT_H
#define HARBORBREEZEHUB_RFDATARECEIVEDEVENT_H

#include "core.h"
#include "ComponentEventSystem/Event.h"

class RFDataReceivedEvent : public Event
{
private:
    const std::shared_ptr<const std::vector<unsigned int>> buffer_;

public:
    explicit RFDataReceivedEvent(std::shared_ptr<const std::vector<unsigned int>> buffer)
        : buffer_(std::move(buffer))
    {
    }

    ~RFDataReceivedEvent() override = default;

    [[nodiscard]] const std::shared_ptr<const std::vector<unsigned int>>& getData() const
    {
        return buffer_;
    }

    [[nodiscard]] EventType type() const override
    {
        return "RFDataReceivedEvent";
    }
};

#endif //HARBORBREEZEHUB_RFDATARECEIVEDEVENT_H
