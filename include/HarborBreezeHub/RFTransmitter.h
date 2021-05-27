//
// Created by enlil on 4/19/21.
//

#ifndef HARBORBREEZEHUB_RF_TRANSMITTER_H
#define HARBORBREEZEHUB_RF_TRANSMITTER_H

#include "ComponentEventSystem/Component.h"

#include <string>

class RFTransmitter : public Component
{
private:
    const int pinNumber;

public:
    RFTransmitter(std::string instanceName, int pinNumber);
    ~RFTransmitter();

    void doWork() override;
    int getPinNumber() const;

    ComponentType type() const override
    {
        return "RFTransmitter";
    }
};

#endif //HARBORBREEZEHUB_RF_TRANSMITTER_H
