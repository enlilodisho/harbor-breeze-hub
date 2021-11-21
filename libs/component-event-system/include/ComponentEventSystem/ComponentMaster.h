#ifndef COMPONENTEVENTSYSTEM_COMPONENT_MASTER_H
#define COMPONENTEVENTSYSTEM_COMPONENT_MASTER_H

#include "Component.h"
#include "EventDispatcher.h"

#include <thread>
#include <unordered_map>
#include <vector>

class ComponentMaster
{
private:
    // Map giving the thread each component is running on
    std::unordered_map<Component*, std::thread> componentThreads_;
    // Boolean stating whether master component has started
    bool running_ = false;

    // Starts a component
    Result startComponentOnNewThread(Component* component);
    // This method is invoked on the running component's thread.
    void component_thread(Component* component);

protected:
    // Stores list of all added components
    std::vector<std::unique_ptr<Component>> components_;
    // Event dispatcher handles event posting and subscriptions
    std::unique_ptr<EventDispatcher> eventDispatcher_;

public:
    ComponentMaster();
    ~ComponentMaster();

    // Starts all components
    Result start();
    // Adds a component. If component master is already started,
    // then the newly added component is started immediately.
    Result addComponent(std::unique_ptr<Component> component);
    // Returns whether component master is running or not
    inline bool running() const
    {
        return running_;
    }
    // Returns instance to event dispatcher
    inline EventDispatcher& getEventDispatcher() const
    {
        return *eventDispatcher_;
    }
};

#endif // COMPONENTEVENTSYSTEM_COMPONENT_MASTER_H