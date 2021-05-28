//
// Created by enlil on 5/27/21.
//

#ifndef HARBORBREEZEHUB_RFDATARECEIVEDEVENT_H
#define HARBORBREEZEHUB_RFDATARECEIVEDEVENT_H

#include "ComponentEventSystem/Event.h"

#include <string>
#include <vector>

class RFDataReceivedEvent : public Event
{
private:
    std::string dataLabel_;
    std::vector<unsigned int> data_;

public:
    RFDataReceivedEvent(std::string dataLabel, const std::vector<unsigned int> data)
        : dataLabel_(dataLabel), data_(data) {}
    ~RFDataReceivedEvent() {}

    std::string getDataLabel() const
    {
        return dataLabel_;
    }

    const std::vector<unsigned int>& getData() const
    {
        return data_;
    }

    EventType type() const override
    {
        return "RFDataReceivedEvent";
    }
};

#endif //HARBORBREEZEHUB_RFDATARECEIVEDEVENT_H
