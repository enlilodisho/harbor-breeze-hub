#ifndef COMPONENTEVENTSYSTEM_COMPONENT_H
#define COMPONENTEVENTSYSTEM_COMPONENT_H

#include "ComponentType.h"
#include "Event.h"
#include "EventDispatcher.h"
#include "Result.h"

#include <memory>
#include <string>

class Component
{
friend class ComponentMaster;

private:
    std::string instanceName_;

protected:
    EventDispatcher* eventDispatcher_ = nullptr;

    // Invoked when component first starts up
    virtual void onStart() {}
    // Invoked when component stops
    virtual void onStop() {}
    // Invoked when an event this component is subscribed to is received
    virtual void onEvent(Component* sender, std::shared_ptr<Event> event) {}
    // Invoked in loop after component events are processed
    virtual void doWork() {}

public:
    explicit Component(const std::string& instanceName) : instanceName_(instanceName) {}
    virtual ~Component() = default;

    // Sets the event dispatcher this component should use.
    void setEventDispatcher(EventDispatcher* eventDispatcher)
    {
        eventDispatcher_ = eventDispatcher;
    }
    // Returns whether the provided event dispatcher is the dispatcher used
    // by this component.
    bool isEventDispatcher(EventDispatcher* eventDispatcher) const
    {
        return eventDispatcher == eventDispatcher_;
    }

    [[nodiscard]] const std::string& instanceName() const
    {
        return instanceName_;
    }
    [[nodiscard]] virtual ComponentType type() const = 0;
};

#endif // COMPONENTEVENTSYSTEM_COMPONENT_H