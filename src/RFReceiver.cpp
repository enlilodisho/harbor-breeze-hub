//
// Created by enlil on 4/19/21.
//

#include "RFReceiver.h"

#include <sstream>
#include <stdexcept>
#include <stdlib.h>
#include <wiringPi.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <errno.h>

std::unordered_map<int, RFReceiver*> RFReceiver::registeredReceivers;

RFReceiver::RFReceiver(std::string instanceName, int pinNumber)
    : pinNumber(pinNumber), Component(instanceName)
{
    // Check if another rf receiver component already has been created for this pin,
    // If not then instantiate and add to registeredReceivers map.
    auto it = registeredReceivers.find(pinNumber);
    if (it != registeredReceivers.end())
    {
        throw std::runtime_error("RF Receiver component already initialized on pin number.");
    }
    pinMode(pinNumber, INPUT);

    // set up interrupt
    switch (pinNumber)
    {
        case 1:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_1);
            break;
        case 2:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_2);
            break;
        case 3:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_3);
            break;
        case 4:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_4);
            break;
        case 5:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_5);
            break;
        case 6:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_6);
            break;
        case 7:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_7);
            break;
        case 8:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_8);
            break;
        case 9:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_9);
            break;
        case 10:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_10);
            break;
        case 11:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_11);
            break;
        case 12:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_12);
            break;
        case 13:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_13);
            break;
        case 14:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_14);
            break;
        case 15:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_15);
            break;
        case 16:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_16);
            break;
        case 17:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_17);
            break;
        case 18:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_18);
            break;
        case 19:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_19);
            break;
        case 20:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_20);
            break;
        case 21:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_21);
            break;
        case 22:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_22);
            break;
        case 23:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_23);
            break;
        case 24:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_24);
            break;
        case 25:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_25);
            break;
        case 26:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_26);
            break;
        case 27:
            wiringPiISR(pinNumber, INT_EDGE_RISING, &RFReceiver::interrupt_callback_pin_27);
            break;
        default:
            throw std::runtime_error("Constructed with unknown pin number.");
    }
    registeredReceivers[pinNumber] = this;

    // initialize last interrupt time point
    last_interrupt = clock::now();
}

RFReceiver::~RFReceiver()
{
    pinMode(pinNumber, INPUT);

    // Remove pin number and rf receiver instance from registeredReceivers
    registeredReceivers.erase(pinNumber);
}

void RFReceiver::doWork()
{
}

void RFReceiver::handle_interrupt_callback()
{
    clock::time_point now = clock::now();
    unsigned long interval = std::chrono::duration_cast<std::chrono::microseconds>(now - last_interrupt).count();
    last_interrupt = now;
}

int RFReceiver::getPinNumber() const
{
    return pinNumber;
}

void RFReceiver::interrupt_callback(int pin)
{
    auto it = registeredReceivers.find(pin);
    if (it != registeredReceivers.end())
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