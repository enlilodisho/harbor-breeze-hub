//
// Created by enlil on 11/21/21.
//

#ifndef HARBORBREEZEHUB_TRANSMITRFDATAENDEVENT_H
#define HARBORBREEZEHUB_TRANSMITRFDATAENDEVENT_H

#include "core.h"
#include "TransmitRFDataRequestEvent.h"

class TransmitRFDataEndEvent : public Event
{
private:
    const std::shared_ptr<const TransmitRFDataRequestEvent> requestEvent_;

public:
    explicit TransmitRFDataEndEvent(std::shared_ptr<const TransmitRFDataRequestEvent> requestEvent)
            : requestEvent_(std::move(requestEvent))
    {
    }
    ~TransmitRFDataEndEvent() override = default;

    [[nodiscard]] const std::shared_ptr<const TransmitRFDataRequestEvent>& getRequestEvent() const
    {
        return requestEvent_;
    }

    [[nodiscard]] EventType type() const override
    {
        return "TransmitRFDataEndEvent";
    }
};

#endif //HARBORBREEZEHUB_TRANSMITRFDATAENDEVENT_H
