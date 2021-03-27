#include "ComponentMaster.h"

#include <functional>
#include <iostream> // TODO temp - replace with Logger

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
    printf("Starting %s '%s'...", component->type(), component->instanceName().c_str());
    component->onStart();

    while (running_)
    {
        component->doWork();
    }

    printf("Stopping %s '%s'...", component->type(), component->instanceName().c_str());
    component->onStop();
}