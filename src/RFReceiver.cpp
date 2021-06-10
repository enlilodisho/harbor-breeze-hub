//
// Created by enlil on 4/19/21.
//
// Based on: rc-switch (https://github.com/sui77/rc-switch)

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

const unsigned int RFReceiver::MIN_SEPARATION_TIMING = 4300;
const unsigned int RFReceiver::MIN_TIMING = 50;
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
    /*
    received_mutex_.lock();
    size_t numReceived = received_.size();
    received_mutex_.unlock();
    std::unique_ptr<RFDataReceivedEvent> receivedDataEvent =
         std::make_unique<RFDataReceivedEvent>(dataReceived_it->first, dataReceived_it->second);
    if (eventDispatcher_)
    {
        eventDispatcher_->post(this, std::move(receivedDataEvent));
    }
     */
}

inline unsigned int RFReceiver::diff(unsigned int a, unsigned int b)
{
    return ((a > b) ? (a - b) : (b - a));
}

bool RFReceiver::receiveProtocol(const RFProtocol& rfProtocol, unsigned long* outData)
{
    unsigned long data = 0; // the received data
    // Assume longer pulse length was captured in timings[0]
    const unsigned int syncLength = (rfProtocol.syncFactor.low > rfProtocol.syncFactor.high) ? rfProtocol.syncFactor.low : rfProtocol.syncFactor.high;
    const unsigned int delay = timings_[0] / syncLength;
    const unsigned int delayTolerance = delay * receiveTolerance_ / 100;

    const unsigned int firstDataTiming = (rfProtocol.invertedSignal) ? 2 : 1;
    // Read signal data
    for (unsigned int i = firstDataTiming; i < timings_.size() - 1; i+=2)
    {
        data <<= 1;
        if (diff(timings_[i], delay * rfProtocol.zero.high) < delayTolerance &&
            diff(timings_[i + 1], delay * rfProtocol.zero.low) < delayTolerance)
        {
            std::cout << "0";
            // zero
        }
        else if (diff(timings_[i], delay * rfProtocol.one.high) < delayTolerance &&
                 diff(timings_[i + 1], delay * rfProtocol.one.low) < delayTolerance)
        {
            // one
            std::cout << "1";
            data |= 1;
        }
        else
        {
            // Failed
            std::cout << "Failed to process signal\n";
            return false;
        }
    }
    std::cout << std::endl;
    *outData = data;
    return true;
}

void RFReceiver::handle_interrupt_callback(const clock::time_point& now)
{
    unsigned long interval = std::chrono::duration_cast<std::chrono::microseconds>(now - last_interrupt_).count();
    last_interrupt_ = now;

    // Ignore short timings
    if (interval < MIN_TIMING)
    {
        return;
    }

    if (interval >= MIN_SEPARATION_TIMING)
    {
        // A long gap between signal level change occurred.
        // This could be the gap between 2 transmissions of same signal.
        if (timingsSignalCount_ == 0 || (timings_.size() > 7 && diff(interval, timings_[0]) < 200))
        {
            // This long signal is close in length to the long signal which came before.
            // Thus it may indeed be a gap between two transmissions, assuming that the
            // sender sends the signal multiple times with the same gap between them.
            timingsSignalCount_++;
            if (timingsSignalCount_ == 2)
            {
                // print out signal timings
                std::cout << "Received: ";
                for (unsigned int timing : timings_)
                {
                    std::cout << timing << ", ";
                }
                std::cout << std::endl;

                // Process received signal
                for (unsigned int i = 0; i < rfProtocols_.size(); i++)
                {
                    unsigned long data;
                    std::cout << "attempt protocol " << i << std::endl;
                    if (receiveProtocol(rfProtocols_[i], &data))
                    {
                        std::cout << "received protocol " << i << ", data: " << data << "\n";
                        /*received_mutex_.lock();
                        received_.push(data);
                        received_mutex_.unlock();*/
                        break;
                    }
                }
                timingsSignalCount_ = 0;
            }
        }
        timings_.clear();
    }

    // Check if overflow signal
    if (timings_.size() >= MAX_TIMINGS)
    {
        timings_.clear();
    }
    // Add timing to timings_ vector
    timings_.push_back(interval);
}

/*
void RFReceiver::receive(const unsigned long data)
{
    received_mutex_.lock();
    // TODO
    received_mutex_.unlock();
}
*/

int RFReceiver::getPinNumber() const
{
    return pinNumber_;
}

void RFReceiver::interrupt_callback(int pin, const clock::time_point& now)
{
    auto it = registeredReceivers_.find(pin);
    if (it != registeredReceivers_.end())
    {
        it->second->handle_interrupt_callback(now);
    }
}

void RFReceiver::interrupt_callback_pin_1()
{
    clock::time_point now = clock::now();
    interrupt_callback(1, now);
}

void RFReceiver::interrupt_callback_pin_2()
{
    clock::time_point now = clock::now();
    interrupt_callback(2, now);
}

void RFReceiver::interrupt_callback_pin_3()
{
    clock::time_point now = clock::now();
    interrupt_callback(3, now);
}

void RFReceiver::interrupt_callback_pin_4()
{
    clock::time_point now = clock::now();
    interrupt_callback(4, now);
}

void RFReceiver::interrupt_callback_pin_5()
{
    clock::time_point now = clock::now();
    interrupt_callback(5, now);
}

void RFReceiver::interrupt_callback_pin_6()
{
    clock::time_point now = clock::now();
    interrupt_callback(6, now);
}

void RFReceiver::interrupt_callback_pin_7()
{
    clock::time_point now = clock::now();
    interrupt_callback(7, now);
}

void RFReceiver::interrupt_callback_pin_8()
{
    clock::time_point now = clock::now();
    interrupt_callback(8, now);
}

void RFReceiver::interrupt_callback_pin_9()
{
    clock::time_point now = clock::now();
    interrupt_callback(9, now);
}

void RFReceiver::interrupt_callback_pin_10()
{
    clock::time_point now = clock::now();
    interrupt_callback(10, now);
}

void RFReceiver::interrupt_callback_pin_11()
{
    clock::time_point now = clock::now();
    interrupt_callback(11, now);
}

void RFReceiver::interrupt_callback_pin_12()
{
    clock::time_point now = clock::now();
    interrupt_callback(12, now);
}

void RFReceiver::interrupt_callback_pin_13()
{
    clock::time_point now = clock::now();
    interrupt_callback(13, now);
}

void RFReceiver::interrupt_callback_pin_14()
{
    clock::time_point now = clock::now();
    interrupt_callback(14, now);
}

void RFReceiver::interrupt_callback_pin_15()
{
    clock::time_point now = clock::now();
    interrupt_callback(15, now);
}

void RFReceiver::interrupt_callback_pin_16()
{
    clock::time_point now = clock::now();
    interrupt_callback(16, now);
}

void RFReceiver::interrupt_callback_pin_17()
{
    clock::time_point now = clock::now();
    interrupt_callback(17, now);
}

void RFReceiver::interrupt_callback_pin_18()
{
    clock::time_point now = clock::now();
    interrupt_callback(18, now);
}

void RFReceiver::interrupt_callback_pin_19()
{
    clock::time_point now = clock::now();
    interrupt_callback(19, now);
}

void RFReceiver::interrupt_callback_pin_20()
{
    clock::time_point now = clock::now();
    interrupt_callback(20, now);
}

void RFReceiver::interrupt_callback_pin_21()
{
    clock::time_point now = clock::now();
    interrupt_callback(21, now);
}

void RFReceiver::interrupt_callback_pin_22()
{
    clock::time_point now = clock::now();
    interrupt_callback(22, now);
}

void RFReceiver::interrupt_callback_pin_23()
{
    clock::time_point now = clock::now();
    interrupt_callback(23, now);
}

void RFReceiver::interrupt_callback_pin_24()
{
    clock::time_point now = clock::now();
    interrupt_callback(24, now);
}

void RFReceiver::interrupt_callback_pin_25()
{
    clock::time_point now = clock::now();
    interrupt_callback(25, now);
}

void RFReceiver::interrupt_callback_pin_26()
{
    clock::time_point now = clock::now();
    interrupt_callback(26, now);
}

void RFReceiver::interrupt_callback_pin_27()
{
    clock::time_point now = clock::now();
    interrupt_callback(27, now);
}