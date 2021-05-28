//
// Created by enlil on 4/19/21.
//

#include "RFReceiver.h"
#include "RFDataReceivedEvent.h"

#include <limits>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>
#include <wiringPi.h>

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <errno.h>

const unsigned int RFReceiver::MIN_TIMING = 5;
const unsigned int RFReceiver::TIMING_MATCH_THRESHOLD = 50;

std::unordered_map<int, RFReceiver*> RFReceiver::registeredReceivers_;

RFReceiver::RFReceiver(std::string instanceName, int pinNumber)
    : pinNumber_(pinNumber), Component(instanceName)
{
    // Check if another rf receiver component already has been created for this pin,
    // If not then instantiate and add to registeredReceivers_ map.
    auto it = registeredReceivers_.find(pinNumber_);
    if (it != registeredReceivers_.end())
    {
        throw std::runtime_error("RF Receiver component already initialized on pin number.");
    }
    pinMode(pinNumber_, INPUT);

    // set up interrupt
    switch (pinNumber_)
    {
        case 1:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_1);
            break;
        case 2:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_2);
            break;
        case 3:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_3);
            break;
        case 4:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_4);
            break;
        case 5:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_5);
            break;
        case 6:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_6);
            break;
        case 7:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_7);
            break;
        case 8:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_8);
            break;
        case 9:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_9);
            break;
        case 10:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_10);
            break;
        case 11:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_11);
            break;
        case 12:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_12);
            break;
        case 13:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_13);
            break;
        case 14:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_14);
            break;
        case 15:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_15);
            break;
        case 16:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_16);
            break;
        case 17:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_17);
            break;
        case 18:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_18);
            break;
        case 19:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_19);
            break;
        case 20:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_20);
            break;
        case 21:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_21);
            break;
        case 22:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_22);
            break;
        case 23:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_23);
            break;
        case 24:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_24);
            break;
        case 25:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_25);
            break;
        case 26:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_26);
            break;
        case 27:
            wiringPiISR(pinNumber_, INT_EDGE_BOTH, &RFReceiver::interrupt_callback_pin_27);
            break;
        default:
            throw std::runtime_error("Constructed with unknown pin number.");
    }
    registeredReceivers_[pinNumber_] = this;

    // initialize last interrupt time point
    last_interrupt_ = clock::now();
}

RFReceiver::~RFReceiver()
{
    pinMode(pinNumber_, INPUT);

    // Remove pin number and rf receiver instance from registeredReceivers_
    registeredReceivers_.erase(pinNumber_);
}

void RFReceiver::doWork()
{
    received_mutex_.lock();
    size_t numReceived = received_.size();
    received_mutex_.unlock();

    processReceivedData(numReceived);
    for (auto it = possibleMatches_.begin(); it != possibleMatches_.end();)
    {
        if (it->second.empty())
        {
            // matched
            auto dataReceived_it = dataToListenFor_.find(it->first);
            if (dataReceived_it != dataToListenFor_.end())
            {
                std::unique_ptr<RFDataReceivedEvent> receivedDataEvent =
                        std::make_unique<RFDataReceivedEvent>(dataReceived_it->first, dataReceived_it->second);
                if (eventDispatcher_)
                {
                    eventDispatcher_->post(this, std::move(receivedDataEvent));
                }
            }
            it = possibleMatches_.erase(it);
        }
        else
        {
            it++;
        }
    }
}

bool RFReceiver::doesTimingMatch(unsigned int desired, unsigned int actual)
{
    unsigned int min = std::numeric_limits<unsigned int>::min();
    unsigned int max = std::numeric_limits<unsigned int>::max();
    if (desired > TIMING_MATCH_THRESHOLD)
    {
        min = desired - TIMING_MATCH_THRESHOLD;
    }
    if (desired < max - TIMING_MATCH_THRESHOLD)
    {
        max = desired + TIMING_MATCH_THRESHOLD;
    }
    return actual >= min && actual <= max;
}

void RFReceiver::processReceivedData(size_t numReceived, size_t numProcessed)
{
    if (numProcessed >= numReceived)
    {
        return;
    }

    for (auto possibleMatches_it = possibleMatches_.begin(); possibleMatches_it != possibleMatches_.end();)
    {
        if (!possibleMatches_it->second.empty())
        {
            if (doesTimingMatch(possibleMatches_it->second.front(), received_.front()))
            {
                possibleMatches_it->second.pop_front();
                possibleMatches_it++;
            }
            else if (received_.front() <= MIN_TIMING)
            {
                possibleMatches_it++;
            }
            else
            {
                possibleMatches_it = possibleMatches_.erase(possibleMatches_it);
            }
        }
        else
        {
            possibleMatches_it++;
        }
    }

    for (auto data_it = dataToListenFor_.begin(); data_it != dataToListenFor_.end(); data_it++)
    {
        if (doesTimingMatch(data_it->second[0], received_.front()))
        {
            std::deque<unsigned int> data;
            for (size_t i = 1; i < data_it->second.size(); i++)
            {
                data.push_back(data_it->second[i]);
            }
            possibleMatches_.push_back(std::make_pair(data_it->first, data));
        }
    }

    received_mutex_.lock();
    received_.pop();
    received_mutex_.unlock();
    processReceivedData(numReceived, numProcessed + 1);
}

void RFReceiver::handle_interrupt_callback()
{
    clock::time_point now = clock::now();
    unsigned long interval = std::chrono::duration_cast<std::chrono::microseconds>(now - last_interrupt_).count();
    last_interrupt_ = now;

    received_mutex_.lock();
    received_.push((unsigned int)interval);
    received_mutex_.unlock();
}

Result RFReceiver::listenForData(const std::string& dataLabel, const std::vector<unsigned int>& data)
{
    auto it = dataToListenFor_.find(dataLabel);
    if (it == dataToListenFor_.end())
    {
        dataToListenFor_[dataLabel] = data;
        return Result(true);
    }
    else
    {
        return Result(false, "already listening to data with provided dataLabel ('%s')", dataLabel.c_str());
    }
}

bool RFReceiver::stopListeningForData(const std::string& dataLabel)
{
    size_t numErased = dataToListenFor_.erase(dataLabel);
    return numErased > 0;
}

void RFReceiver::receive(const std::vector<unsigned int>& data)
{
    received_mutex_.lock();
    for (size_t i = 0; i < data.size(); i++)
    {
        received_.push(data[i]);
    }
    received_mutex_.unlock();
}

int RFReceiver::getPinNumber() const
{
    return pinNumber_;
}

void RFReceiver::interrupt_callback(int pin)
{
    auto it = registeredReceivers_.find(pin);
    if (it != registeredReceivers_.end())
    {
        it->second->handle_interrupt_callback();
    }
}

void RFReceiver::interrupt_callback_pin_1()
{
    interrupt_callback(1);
}

void RFReceiver::interrupt_callback_pin_2()
{
    interrupt_callback(2);
}

void RFReceiver::interrupt_callback_pin_3()
{
    interrupt_callback(3);
}

void RFReceiver::interrupt_callback_pin_4()
{
    interrupt_callback(4);
}

void RFReceiver::interrupt_callback_pin_5()
{
    interrupt_callback(5);
}

void RFReceiver::interrupt_callback_pin_6()
{
    interrupt_callback(6);
}

void RFReceiver::interrupt_callback_pin_7()
{
    interrupt_callback(7);
}

void RFReceiver::interrupt_callback_pin_8()
{
    interrupt_callback(8);
}

void RFReceiver::interrupt_callback_pin_9()
{
    interrupt_callback(9);
}

void RFReceiver::interrupt_callback_pin_10()
{
    interrupt_callback(10);
}

void RFReceiver::interrupt_callback_pin_11()
{
    interrupt_callback(11);
}

void RFReceiver::interrupt_callback_pin_12()
{
    interrupt_callback(12);
}

void RFReceiver::interrupt_callback_pin_13()
{
    interrupt_callback(13);
}

void RFReceiver::interrupt_callback_pin_14()
{
    interrupt_callback(14);
}

void RFReceiver::interrupt_callback_pin_15()
{
    interrupt_callback(15);
}

void RFReceiver::interrupt_callback_pin_16()
{
    interrupt_callback(16);
}

void RFReceiver::interrupt_callback_pin_17()
{
    interrupt_callback(17);
}

void RFReceiver::interrupt_callback_pin_18()
{
    interrupt_callback(18);
}

void RFReceiver::interrupt_callback_pin_19()
{
    interrupt_callback(19);
}

void RFReceiver::interrupt_callback_pin_20()
{
    interrupt_callback(20);
}

void RFReceiver::interrupt_callback_pin_21()
{
    interrupt_callback(21);
}

void RFReceiver::interrupt_callback_pin_22()
{
    interrupt_callback(22);
}

void RFReceiver::interrupt_callback_pin_23()
{
    interrupt_callback(23);
}

void RFReceiver::interrupt_callback_pin_24()
{
    interrupt_callback(24);
}

void RFReceiver::interrupt_callback_pin_25()
{
    interrupt_callback(25);
}

void RFReceiver::interrupt_callback_pin_26()
{
    interrupt_callback(26);
}

void RFReceiver::interrupt_callback_pin_27()
{
    interrupt_callback(27);
}