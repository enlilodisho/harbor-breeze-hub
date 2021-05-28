#ifndef HARBORBREEZEHUB_FAKECOMPONENT_H
#define HARBORBREEZEHUB_FAKECOMPONENT_H

#include "ComponentEventSystem/Component.h"

#include <string>

class FakeComponent : public Component
{
public:
    FakeComponent(std::string instanceName) : Component(instanceName) {}
    ~FakeComponent() {}

    ComponentType type() const override
    {
        return "FakeComponent";
    }
};

#endif //HARBORBREEZEHUB_FAKECOMPONENT_H
