//
// Created by enlil on 11/21/21.
//

#include "ComponentEventSystem/Component.h"
#include "ComponentEventSystem/ComponentMaster.h"
#include "ComponentEventSystem/EventDispatcher.h"

#ifndef HARBORBREEZEHUB_COMPONENTMASTERFORTESTS_H
#define HARBORBREEZEHUB_COMPONENTMASTERFORTESTS_H

class ComponentMasterForTests : public ComponentMaster
{
public:
    void runOnStartOnAllComponents()
    {
        for (auto& component : components_)
        {
            component->onStart();
        }
    }

    void runOnStopOnAllComponents()
    {
        for (auto& component : components_)
        {
            component->onStop();
        }
    }

    void setEventDispatcher(std::unique_ptr<EventDispatcher> eventDispatcher)
    {
        eventDispatcher_ = std::move(eventDispatcher);
    }

    void update()
    {
        for (auto& component : components_)
        {
            // get all events to send to this component
            std::vector<std::pair<Component*, std::shared_ptr<Event>>> eventsToSend;
            getEventDispatcher().eventsForComponent_mutex.lock();
            auto events_it = getEventDispatcher().eventsForComponent_.find(component.get());
            if (events_it != getEventDispatcher().eventsForComponent_.end())
            {
                for (auto& event : events_it->second)
                {
                    eventsToSend.emplace_back(
                            std::pair<Component*, std::shared_ptr<Event>>{event.first,std::move(event.second)});
                }
                events_it->second.clear();
            }
            getEventDispatcher().eventsForComponent_mutex.unlock();

            // send events
            for (auto& event : eventsToSend)
            {
                component->onEvent(event.first, std::move(event.second));
            }
            // do work
            component->doWork();
        }
    }
};

#endif //HARBORBREEZEHUB_COMPONENTMASTERFORTESTS_H
