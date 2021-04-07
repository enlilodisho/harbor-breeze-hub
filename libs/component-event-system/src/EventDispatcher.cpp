#include "EventDispatcher.h"
#include <iostream>
#include <utility>

void EventDispatcher::post(Component* sender, std::unique_ptr<Event> event)
{
    std::unordered_set<Component*> componentsToReceiveEvent;

    // get all subscribers who subscribed to this event from this particular sender
    subscribersToSender_mutex.lock();
    auto sender_subscribers_it = subscribersToSender_.find({event->type(),sender});
    if (sender_subscribers_it != subscribersToSender_.end())
    {
        componentsToReceiveEvent = sender_subscribers_it->second;
    }
    subscribersToSender_mutex.unlock();

    // get all subscribers who subscribed to this event from senders of this type
    subscribersToSenderType_mutex.lock();
    auto senderType_subscribers_it = subscribersToSenderType_.find({event->type(),sender->type()});
    if (senderType_subscribers_it != subscribersToSenderType_.end())
    {
        componentsToReceiveEvent.insert(senderType_subscribers_it->second.begin(),
            senderType_subscribers_it->second.end());
    }
    subscribersToSenderType_mutex.unlock();

    // post event for all components
    std::shared_ptr<Event> event_shared = std::move(event);
    eventsForComponent_mutex.lock();
    for (Component* component : componentsToReceiveEvent)
    {
        if (component != sender)
        {
            eventsForComponent_[component].push_back({sender,event_shared});
        }
    }
    eventsForComponent_mutex.unlock();
}

void EventDispatcher::subscribe(Component* component, EventType eventType, Component* owner)
{
    subscribersToSender_mutex.lock();
    subscribersToSender_[{eventType,owner}].insert(component);
    subscribersToSender_mutex.unlock();
}

void EventDispatcher::subscribe(Component* component, EventType eventType, ComponentType ownerType)
{
    subscribersToSenderType_mutex.lock();
    subscribersToSenderType_[{eventType,ownerType}].insert(component);
    subscribersToSenderType_mutex.unlock();
}