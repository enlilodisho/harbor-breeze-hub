//
// Created by enlil on 4/19/21.
//

#include "RFTransmitter.h"

#include <wiringPi.h>

RFTransmitter::RFTransmitter(std::string instanceName, int pinNumber)
    : pinNumber(pinNumber), Component(instanceName)
{
    pinMode(pinNumber, OUTPUT);
}

RFTransmitter::~RFTransmitter()
{
    pinMode(pinNumber, INPUT);
}

void RFTransmitter::doWork()
{

}

int RFTransmitter::getPinNumber() const
{
    return pinNumber;
}