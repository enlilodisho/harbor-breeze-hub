//
// Created by enlil on 11/19/21.
//

#ifndef HARBORBREEZEHUB_RFTRANSMITTER_H
#define HARBORBREEZEHUB_RFTRANSMITTER_H

#include "core.h"
#include "TransmitRFDataRequestEvent.h"

#include <chrono>
#include <queue>

class RFTransmitter : public Component
{
private:
    typedef std::chrono::steady_clock my_clock;
    const int pinNumber_;

    std::queue<std::shared_ptr<TransmitRFDataRequestEvent>> transmissionQueue_;
    // vars for current transmission
    std::shared_ptr<TransmitRFDataRequestEvent> currentTransmissionEvent_;
    my_clock::time_point lastTransmissionTime_;
    size_t transmissionIndex_;
    size_t transmitCount_;
    bool nextTransmissionHighType_;
    unsigned int currentWaitTime_;

protected:
    void onEvent(Component* sender, std::shared_ptr<Event> event) override;
    void doWork() override;

public:
    RFTransmitter(const std::string& instanceName, int pinNumber);
    ~RFTransmitter() override;

    [[nodiscard]] ComponentType type() const override
    {
        return "RFTransmitter";
    }
};

#endif //HARBORBREEZEHUB_RFTRANSMITTER_H
