//
// Created by enlil on 5/27/21.
//

#ifndef HARBORBREEZEHUB_RFDATAEVENT_H
#define HARBORBREEZEHUB_RFDATAEVENT_H

#include "ComponentEventSystem/Event.h"

#include <string>
#include <vector>

class RFDataEvent : public Event
{
private:
    std::string dataLabel_;
    std::vector<unsigned int> data_;

public:
    RFDataEvent(std::string dataLabel, const std::vector<unsigned int> data)
        : dataLabel_(dataLabel), data_(data) {}
    ~RFDataEvent() {}

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
        return "RFDataEvent";
    }
};

#endif //HARBORBREEZEHUB_RFDATAEVENT_H
