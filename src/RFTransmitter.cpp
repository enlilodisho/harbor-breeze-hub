//
// Created by enlil on 4/19/21.
//

#include "RFTransmitter.h"
#include "RFDataTransmittedEvent.h"

#include <chrono>
#include <wiringPi.h>
#include <iostream>

RFTransmitter::RFTransmitter(std::string instanceName, int pinNumber)
    : pinNumber_(pinNumber), Component(instanceName)
{
    pinMode(pinNumber_, OUTPUT);
}

RFTransmitter::~RFTransmitter()
{
    pinMode(pinNumber_, INPUT);
}

void RFTransmitter::doWork()
{
    if (!dataToSend_.empty())
    {
        std::vector<unsigned int> data = dataToSend_.front();
        dataToSend_.pop();

        bool isHigh = false;
        size_t data_i = 0;
        size_t dataLength = data.size();
        typedef std::chrono::steady_clock clock;
        clock::time_point lastPowerToggle = clock::now();
        for (; data_i < dataLength; data_i++)
        {
            if (isHigh)
            {
                digitalWrite(pinNumber_, 0);
                isHigh = false;
            }
            else
            {
                digitalWrite(pinNumber_, 1);
                isHigh = true;
            }

            clock::time_point now;
            unsigned long interval;
            do
            {
                now = clock::now();
                interval = std::chrono::duration_cast<std::chrono::microseconds>(now - lastPowerToggle).count();
            }
            while (interval < data[data_i]);

            lastPowerToggle = now;
        }

        digitalWrite(pinNumber_, 0);
        if (eventDispatcher_)
        {
            std::unique_ptr<RFDataTransmittedEvent> transmittedDataEvent =
                std::make_unique<RFDataTransmittedEvent>(data);
            eventDispatcher_->post(this, std::move(transmittedDataEvent));
        }
    }
}

void RFTransmitter::transmit(const std::vector<unsigned int>& data)
{
    dataToSend_.push(data);
}

int RFTransmitter::getPinNumber() const
{
    return pinNumber_;
}