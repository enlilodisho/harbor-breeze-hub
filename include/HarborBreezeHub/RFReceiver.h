//
// Created by enlil on 10/3/21.
//

#ifndef HARBORBREEZEHUB_RFRECEIVER_H
#define HARBORBREEZEHUB_RFRECEIVER_H

#include "core.h"
#include "ComponentEventSystem/Component.h"
#include "InterruptsController.h"

class RFReceiver : public Component
{
private:
    const int pinNumber_;

    void onReceiveInterrupt(const InterruptsController::clock::time_point& timePoint);

public:
    RFReceiver(const std::string& instanceName, int pinNumber);
    ~RFReceiver();

    ComponentType type() const override
    {
        return "RFReceiver";
    }
};

#endif //HARBORBREEZEHUB_RFRECEIVER_H
