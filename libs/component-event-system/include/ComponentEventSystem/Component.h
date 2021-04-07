#ifndef COMPONENTEVENTSYSTEM_COMPONENT_H
#define COMPONENTEVENTSYSTEM_COMPONENT_H

#include "Event.h"
#include "Result.h"

#include <memory>
#include <string>

using ComponentType = std::string;

class Component
{
friend class ComponentMaster;

private:
    std::string instanceName_;

protected:
    // Invoked when component first starts up
    virtual void onStart() {}
    // Invoked when component stops
    virtual void onStop() {}
    // Invoked when an event this component is subscribed to is received
    virtual void onEvent(Component* sender, std::shared_ptr<Event> event) {}
    // Invoked in loop after component events are processed
    virtual void doWork() {}

public:
    Component(const std::string& instanceName) : instanceName_(instanceName) {}
    virtual ~Component() {}

    const std::string& instanceName() const
    {
        return instanceName_;
    }
    virtual ComponentType type() const = 0;
};

#endif // COMPONENTEVENTSYSTEM_COMPONENT_H