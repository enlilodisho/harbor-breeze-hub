#include "ComponentMaster.h"

#include <functional>
#include <iostream> // TODO temp - replace with Logger

ComponentMaster::ComponentMaster()
{
    eventDispatcher_ = std::make_unique<EventDispatcher>();
}

ComponentMaster::~ComponentMaster()
{
    running_ = false; // will cause component threads to end
    // Wait until all threads are done executing
    for (auto componentThread_it = componentThreads_.begin(); componentThread_it != componentThreads_.end(); componentThread_it++)
    {
        componentThread_it->second.join();
    }
}

Result ComponentMaster::addComponent(std::unique_ptr<Component> component)
{
    components_.push_back(std::move(component)); // add component
    if (running_) // start component if component master has already been started
    {
        return startComponentOnNewThread(components_[components_.size() - 1].get());
    }
    return Result(true);
}

Result ComponentMaster::start()
{
    if (running_)
    {
        return Result(false, "component master already started");
    }
    running_ = true;

    Result returnResult(true);
    for (auto& component : components_)
    {
        returnResult += startComponentOnNewThread(component.get());
    }
    return returnResult;
}

Result ComponentMaster::startComponentOnNewThread(Component* component)
{
    if (componentThreads_.find(component) != componentThreads_.end())
    {
        return Result(false, "Unable to start component: thread for component already exists in map");
    }

    // Create thread for component
    auto thread_fn = std::bind(&ComponentMaster::component_thread, this, component);
    componentThreads_[component] = std::thread(thread_fn);
    return Result(true);
}

void ComponentMaster::component_thread(Component* component)
{
    printf("Starting %s '%s'\n", component->type().c_str(), component->instanceName().c_str());
    component->onStart();

    while (running_)
    {
        // get all events to send to this component
        std::vector<std::pair<Component*, std::shared_ptr<Event>>> eventsToSend;
        eventDispatcher_->eventsForComponent_mutex.lock();
        auto events_it = eventDispatcher_->eventsForComponent_.find(component);
        if (events_it != eventDispatcher_->eventsForComponent_.end())
        {
            for (auto& event : events_it->second)
            {
                eventsToSend.push_back({event.first,std::move(event.second)});
            }
            events_it->second.clear();
        }
        eventDispatcher_->eventsForComponent_mutex.unlock();

        // send events
        for (auto& event : eventsToSend)
        {
            component->onEvent(event.first, std::move(event.second));
        }
        // do work
        component->doWork();
    }

    printf("Stopping %s '%s'\n", component->type().c_str(), component->instanceName().c_str());
    component->onStop();
}