//
// Created by enlil on 11/19/21.
//

#ifndef HARBORBREEZEHUB_TRANSMITRFDATAREQUESTEVENT_H
#define HARBORBREEZEHUB_TRANSMITRFDATAREQUESTEVENT_H

#include "core.h"
#include "ComponentEventSystem/Event.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

class TransmitRFDataRequestEvent : public Event
{
private:
    std::string requestUUID_;
    const std::shared_ptr<const std::vector<unsigned int>> timings_;
    const unsigned int restTiming_;
    const size_t repetitions_; // 0 for transmit by begin/end events

public:
    TransmitRFDataRequestEvent(std::shared_ptr<const std::vector<unsigned int>> timings, unsigned int restTiming,
                               size_t repetitions)
                               : timings_(std::move(timings)), restTiming_(restTiming), repetitions_(repetitions)
    {
        std::ostringstream ss;
        boost::uuids::uuid uuid = boost::uuids::random_generator()();
        ss << uuid;
        requestUUID_ = ss.str();
    }

    TransmitRFDataRequestEvent(const TransmitRFDataRequestEvent& original)
        : timings_(original.timings_), restTiming_(original.restTiming_), repetitions_(original.repetitions_)
    {
        // generate new uuid for copy
        std::ostringstream ss;
        boost::uuids::uuid uuid = boost::uuids::random_generator()();
        ss << uuid;
        requestUUID_ = ss.str();
    }

    ~TransmitRFDataRequestEvent() override = default;

    [[nodiscard]] const std::string& getRequestUUID() const
    {
        return requestUUID_;
    }

    [[nodiscard]] const std::shared_ptr<const std::vector<unsigned int>>& getData() const
    {
        return timings_;
    }

    [[nodiscard]] unsigned int getRestTiming() const
    {
        return restTiming_;
    }

    [[nodiscard]] size_t getRepetitions() const
    {
        return repetitions_;
    }

    [[nodiscard]] EventType type() const override
    {
        return "TransmitRFDataRequestEvent";
    }
};

#endif //HARBORBREEZEHUB_TRANSMITRFDATAREQUESTEVENT_H
