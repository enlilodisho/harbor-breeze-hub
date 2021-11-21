#ifndef HARBORBREEZEHUB_EVENTDISPATCHERFORTESTS_H
#define HARBORBREEZEHUB_EVENTDISPATCHERFORTESTS_H

#include "ComponentEventSystem/EventDispatcher.h"

#include <utility>
#include <vector>

class Component;

class EventDispatcherForTests : public EventDispatcher
{
public:
    EventDispatcherForTests() {}
    ~EventDispatcherForTests() {}

    // Gets all pending events for component from dispatcher.
    std::vector<std::pair<Component*,std::shared_ptr<Event>>>
        getEventsForComponent(Component* component)
    {
        std::vector<std::pair<Component*,std::shared_ptr<Event>>> events;

        eventsForComponent_mutex.lock();
        auto it = eventsForComponent_.find(component);
        if (it != eventsForComponent_.end())
        {
            events = it->second;
            //eventsForComponent_.erase(it);
        }
        eventsForComponent_mutex.unlock();
        return events;
    }
};

#endif //HARBORBREEZEHUB_EVENTDISPATCHERFORTESTS_H
