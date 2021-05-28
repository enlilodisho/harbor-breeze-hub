//
// Created by enlil on 5/27/21.
//

#ifndef HARBORBREEZEHUB_RFDATATRANSMITTEDEVENT_H
#define HARBORBREEZEHUB_RFDATATRANSMITTEDEVENT_H

#include "ComponentEventSystem/Event.h"

#include <vector>

class RFDataTransmittedEvent : public Event
{
private:
    std::vector<unsigned int> data_;

public:
    RFDataTransmittedEvent(const std::vector<unsigned int> data)
        : data_(data) {}
    ~RFDataTransmittedEvent() {}

    const std::vector<unsigned int>& getData() const
    {
        return data_;
    }

    EventType type() const override
    {
        return "RFDataTransmittedEvent";
    }
};

#endif //HARBORBREEZEHUB_RFDATATRANSMITTEDEVENT_H
