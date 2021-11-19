//
// Created by enlil on 10/3/21.
//

#include "RFReceiver.h"
#include "RFDataReceivedEvent.h"

#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>

RFReceiver::RFReceiver(const std::string& instanceName, int pinNumber)
    : pinNumber_(pinNumber), Component(instanceName)
{
    // Set the path to the gpio kernel module file
    std::stringstream ss;
    ss << IRQ_TIMINGS_PATH IRQ_TIMINGS_GPIO_FILE_PREFIX << pinNumber_;
    gpioFile_ = ss.str();

    // Make sure that this GPIO pin is not already registered
    // with irq_timings kernel module.
    if (boost::filesystem::exists(gpioFile_))
    {
        throw std::runtime_error("Pin already registered with irq_timings kernel module");
    }

    // Register GPIO pin with irq_timings kernel module
    std::ofstream irq_register;
    irq_register.open(IRQ_TIMINGS_PATH "register");
    irq_register << pinNumber_;
    irq_register.close();

    buffer_ = std::make_shared<std::vector<unsigned int>>();
}

RFReceiver::~RFReceiver()
{
    // Unregister GPIO pin from irq_timings kernel module
    std::ofstream irq_register;
    irq_register.open(IRQ_TIMINGS_PATH "unregister");
    irq_register << pinNumber_;
    irq_register.close();
}

void RFReceiver::doWork()
{
    // read timings from kernel module
    std::ifstream gpio;
    gpio.open(gpioFile_);
    std::string lineBuf;
    while (getline(gpio, lineBuf))
    {
        try
        {
            buffer_->push_back(stoul(lineBuf, nullptr, 10));
        }
        catch (...) { /* Failed to parse timing */ }
    }

    if (!buffer_->empty())
    {
        if (eventDispatcher_ == nullptr)
        {
            throw std::runtime_error("Event dispatcher not set, cannot post RFDataReceivedEvent.");
        }
        eventDispatcher_->post(this, std::move(std::make_unique<RFDataReceivedEvent>(std::move(buffer_))));
        buffer_ = std::make_shared<std::vector<unsigned int>>();
    }
}