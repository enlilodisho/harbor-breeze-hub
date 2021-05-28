#ifndef COMPONENTEVENTSYSTEM_EVENT_DISPATCHER_H
#define COMPONENTEVENTSYSTEM_EVENT_DISPATCHER_H

#include "ComponentType.h"
#include "Event.h"

#include <boost/functional/hash.hpp>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Component;

class EventDispatcher
{
friend class ComponentMaster;
private:
    // map containing all subscribers for a particular event from a particular sender
    std::unordered_map<std::pair<EventType,Component*>, std::unordered_set<Component*>,
        boost::hash<std::pair<EventType,Component*>> > subscribersToSender_;
    // map containing all subscribers for a partiular event from any sender of a certain type
    std::unordered_map<std::pair<EventType,ComponentType>, std::unordered_set<Component*>,
        boost::hash<std::pair<EventType,ComponentType>> > subscribersToSenderType_;
    std::mutex subscribersToSender_mutex;
    std::mutex subscribersToSenderType_mutex;

protected:
    // map containing all outstanding events and their sender to be received by a component
    std::unordered_map<Component*,std::vector<std::pair<Component*,std::shared_ptr<Event>>> > eventsForComponent_;
    std::mutex eventsForComponent_mutex;
    
public:
    EventDispatcher();
    virtual ~EventDispatcher();

    // publish event from component to all event subscribers
    void post(Component* sender, std::unique_ptr<Event> event);
    // subscribe to an event sent from a particular component 
    void subscribe(Component* component, EventType eventType, Component* owner);
    // subscribe to an event sent from any component of a certain type
    void subscribe(Component* component, EventType eventType, ComponentType ownerType);
    
};

#endif // COMPONENTEVENTSYSTEM_EVENT_DISPATCHER_H