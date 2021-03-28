#ifndef COMPONENTEVENTSYSTEM_COMPONENT_H
#define COMPONENTEVENTSYSTEM_COMPONENT_H

#include "Result.h"

#include <string>

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
    // Invoked in loop after component events are processed
    virtual void doWork() {}

public:
    Component(const std::string& instanceName) : instanceName_(instanceName) {}

    const std::string& instanceName() const
    {
        return instanceName_;
    }
    using ComponentType = const char *;
    virtual ComponentType type() const = 0;
};

#endif // COMPONENTEVENTSYSTEM_COMPONENT_H