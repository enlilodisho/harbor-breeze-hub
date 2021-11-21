//
// Created by enlil on 11/19/21.
//

#include "RFTransmitter.h"
#include "TransmitRFDataBeginEvent.h"
#include "TransmitRFDataEndEvent.h"

#include <wiringPi.h>
#include <iostream>

RFTransmitter::RFTransmitter(const std::string& instanceName, int pinNumber)
    : pinNumber_(pinNumber), Component(instanceName)
{
    pinMode(pinNumber_, OUTPUT);
}

RFTransmitter::~RFTransmitter()
{
    digitalWrite(pinNumber_, LOW);
    pinMode(pinNumber_, INPUT);
}

void RFTransmitter::onEvent(Component* sender, std::shared_ptr<Event> event)
{
    if (event->type() == "TransmitRFDataRequestEvent")
    {
        std::shared_ptr<TransmitRFDataRequestEvent> transmitRFDataRequestEvent
            = std::static_pointer_cast<TransmitRFDataRequestEvent>(event);
        transmissionQueue_.push(std::move(transmitRFDataRequestEvent));
    }
}

void RFTransmitter::doWork()
{
    if (currentTransmissionEvent_ == nullptr)
    {
        if (transmissionQueue_.empty())
        {
            return;
        }
        currentTransmissionEvent_ = std::move(transmissionQueue_.front());
        transmissionQueue_.pop();

        transmissionIndex_ = 0;
        transmitCount_ = 0;
        nextTransmissionHighType_ = true;
        currentWaitTime_ = 0;

        // Invoke begin transmission event
        if (eventDispatcher_ != nullptr)
        {
            eventDispatcher_->post(this,
                                   std::make_unique<TransmitRFDataBeginEvent>(currentTransmissionEvent_));
        }

        lastTransmissionTime_ = my_clock::now();
    }

    if (currentWaitTime_ > 0)
    {
        my_clock::time_point curTime = my_clock::now();
        if (std::chrono::duration_cast<std::chrono::microseconds>(curTime - lastTransmissionTime_).count()
            < currentWaitTime_)
        {
            return;
        }
        lastTransmissionTime_ = curTime;
    }

    auto& timings = currentTransmissionEvent_->getData();
    if (transmissionIndex_ < timings->size())
    {
        if (nextTransmissionHighType_)
        {
            digitalWrite(pinNumber_, HIGH);
        }
        else
        {
            digitalWrite(pinNumber_, LOW);
        }
        nextTransmissionHighType_ = !nextTransmissionHighType_;

        currentWaitTime_ = timings->at(transmissionIndex_);
        transmissionIndex_++;
    }
    else
    {
        digitalWrite(pinNumber_, LOW);

        transmitCount_++;
        transmissionIndex_ = 0;
        if (transmitCount_ < currentTransmissionEvent_->getRepetitions()
            || currentTransmissionEvent_->getRepetitions() == 0)
        {
            currentWaitTime_ = currentTransmissionEvent_->getRestTiming();
            nextTransmissionHighType_ = true;
        }
        else
        {
            // Invoke end transmission event
            if (eventDispatcher_ != nullptr)
            {
                eventDispatcher_->post(this,
                                       std::make_unique<TransmitRFDataEndEvent>(currentTransmissionEvent_));
            }

            currentTransmissionEvent_.reset();
        }
    }
}