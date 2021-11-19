//
// Created by enlil on 10/3/21.
//

#ifndef HARBORBREEZEHUB_RFRECEIVER_H
#define HARBORBREEZEHUB_RFRECEIVER_H

#include "core.h"

class RFReceiver : public Component
{
private:
    const int pinNumber_;
    std::string gpioFile_;

    std::shared_ptr<std::vector<unsigned int>> buffer_;

protected:
    void doWork() override;

public:
    RFReceiver(const std::string& instanceName, int pinNumber);
    ~RFReceiver() override;

    [[nodiscard]] ComponentType type() const override
    {
        return "RFReceiver";
    }
};

#endif //HARBORBREEZEHUB_RFRECEIVER_H
