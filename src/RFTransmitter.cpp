//
// Created by enlil on 4/19/21.
//

#include "RFTransmitter.h"

#include <wiringPi.h>

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

}

void RFTransmitter::transmit(const std::vector<unsigned int>& data)
{

}

int RFTransmitter::getPinNumber() const
{
    return pinNumber_;
}