//
// Created by enlil on 11/21/21.
//

#ifndef HARBORBREEZEHUB_TRANSMITRFDATABEGINEVENT_H
#define HARBORBREEZEHUB_TRANSMITRFDATABEGINEVENT_H

#include "core.h"
#include "TransmitRFDataRequestEvent.h"

class TransmitRFDataBeginEvent : public Event
{
private:
    const std::shared_ptr<const TransmitRFDataRequestEvent> requestEvent_;

public:
    explicit TransmitRFDataBeginEvent(std::shared_ptr<const TransmitRFDataRequestEvent> requestEvent)
        : requestEvent_(std::move(requestEvent))
    {
    }
    ~TransmitRFDataBeginEvent() override = default;

    [[nodiscard]] const std::shared_ptr<const TransmitRFDataRequestEvent>& getRequestEvent() const
    {
        return requestEvent_;
    }

    [[nodiscard]] EventType type() const override
    {
        return "TransmitRFDataBeginEvent";
    }
};

#endif //HARBORBREEZEHUB_TRANSMITRFDATABEGINEVENT_H
