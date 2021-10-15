//
// Created by enlil on 10/3/21.
//

#include "RFReceiver.h"

#include <functional>
#include <wiringPi.h>

RFReceiver::RFReceiver(const std::string& instanceName, int pinNumber)
    : pinNumber_(pinNumber), Component(instanceName)
{
    using namespace std::placeholders;
    Result r1 = InterruptsController::registerInterrupt(pinNumber, INT_EDGE_BOTH,
                                                        std::bind(&RFReceiver::onReceiveInterrupt, this, _1));
    if (!r1.success)
    {
        throw std::logic_error(r1.message);
    }
}

RFReceiver::~RFReceiver()
{
    // TODO possible to unregister interrupt?
}

void RFReceiver::onReceiveInterrupt(const InterruptsController::clock::time_point& timePoint)
{

}