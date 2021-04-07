#ifndef COMPONENTEVENTSYSTEM_EVENT_H
#define COMPONENTEVENTSYSTEM_EVENT_H

#include <string>

using EventType = std::string;

class Event
{
public:
    virtual ~Event() {}

    virtual EventType type() const = 0;
};

#endif // COMPONENTEVENTSYSTEM_EVENT_H