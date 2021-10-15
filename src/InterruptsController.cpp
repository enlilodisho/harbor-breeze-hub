//
// Created by enlil on 7/13/21.
//

#include "InterruptsController.h"

#include <wiringPi.h>

InterruptsController* InterruptsController::instance_ = nullptr;

InterruptsController::InterruptsController()
{

}

InterruptsController::~InterruptsController()
{
    delete instance_;
}

Result InterruptsController::registerInterrupt(int pin, int edgeType, const std::function<void(const clock::time_point&)>& interruptHandler)
{
    if (instance_ == nullptr)
    {
        return Result(false, "No singleton instance of InterruptsController exists.");
    }

    // Make sure interrupt has not already been registered
    auto it = instance_->registeredInterrupts_.find(pin);
    if (it != instance_->registeredInterrupts_.end())
    {
        return Result(false, "An interrupt handler is already registered for this pin.");
    }
    // Register interrupt
    InterruptsController::interrupt_callback_fn* internal_callback_function =
            InterruptsController::get_interrupt_callback_function_by_pin(pin);
    if (internal_callback_function == nullptr)
    {
        return Result(false, "No internal callback function available for specified pin.");
    }
    instance_->registeredInterrupts_[pin] = interruptHandler;
    pinMode(pin, INPUT);
    wiringPiISR(pin, edgeType, internal_callback_function);
    return Result(true);
}

void InterruptsController::interrupt_callback(int pin, const clock::time_point& now)
{
    // Find the registered interrupt handler
    auto it = registeredInterrupts_.find(pin);
    if (it != registeredInterrupts_.end())
    {
        // handler has been registered for this interrupt - invoke it
        it->second(now);
    }
}

void InterruptsController::interrupt_callback_pin_1()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(1, now);
    }
}

void InterruptsController::interrupt_callback_pin_2()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(2, now);
    }
}

void InterruptsController::interrupt_callback_pin_3()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(3, now);
    }
}

void InterruptsController::interrupt_callback_pin_4()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(4, now);
    }
}

void InterruptsController::interrupt_callback_pin_5()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(5, now);
    }
}

void InterruptsController::interrupt_callback_pin_6()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(6, now);
    }
}

void InterruptsController::interrupt_callback_pin_7()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(7, now);
    }
}

void InterruptsController::interrupt_callback_pin_8()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(8, now);
    }
}

void InterruptsController::interrupt_callback_pin_9()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(9, now);
    }
}

void InterruptsController::interrupt_callback_pin_10()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(10, now);
    }
}

void InterruptsController::interrupt_callback_pin_11()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(11, now);
    }
}

void InterruptsController::interrupt_callback_pin_12()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(12, now);
    }
}

void InterruptsController::interrupt_callback_pin_13()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(13, now);
    }
}

void InterruptsController::interrupt_callback_pin_14()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(14, now);
    }
}

void InterruptsController::interrupt_callback_pin_15()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(15, now);
    }
}

void InterruptsController::interrupt_callback_pin_16()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(16, now);
    }
}

void InterruptsController::interrupt_callback_pin_17()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(17, now);
    }
}

void InterruptsController::interrupt_callback_pin_18()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(18, now);
    }
}

void InterruptsController::interrupt_callback_pin_19()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(19, now);
    }
}

void InterruptsController::interrupt_callback_pin_20()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(20, now);
    }
}

void InterruptsController::interrupt_callback_pin_21()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(21, now);
    }
}

void InterruptsController::interrupt_callback_pin_22()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(22, now);
    }
}

void InterruptsController::interrupt_callback_pin_23()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(23, now);
    }
}

void InterruptsController::interrupt_callback_pin_24()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(24, now);
    }
}

void InterruptsController::interrupt_callback_pin_25()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(25, now);
    }
}

void InterruptsController::interrupt_callback_pin_26()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(26, now);
    }
}

void InterruptsController::interrupt_callback_pin_27()
{
    clock::time_point now = clock::now();
    if (instance_)
    {
        instance_->interrupt_callback(27, now);
    }
}

InterruptsController::interrupt_callback_fn* InterruptsController::get_interrupt_callback_function_by_pin(int pin)
{
    switch (pin)
    {
        case 1:
            return &InterruptsController::interrupt_callback_pin_1;
            break;
        case 2:
            return &InterruptsController::interrupt_callback_pin_2;
            break;
        case 3:
            return &InterruptsController::interrupt_callback_pin_3;
            break;
        case 4:
            return &InterruptsController::interrupt_callback_pin_4;
            break;
        case 5:
            return &InterruptsController::interrupt_callback_pin_5;
            break;
        case 6:
            return &InterruptsController::interrupt_callback_pin_6;
            break;
        case 7:
            return &InterruptsController::interrupt_callback_pin_7;
            break;
        case 8:
            return &InterruptsController::interrupt_callback_pin_8;
            break;
        case 9:
            return &InterruptsController::interrupt_callback_pin_9;
            break;
        case 10:
            return &InterruptsController::interrupt_callback_pin_10;
            break;
        case 11:
            return &InterruptsController::interrupt_callback_pin_11;
            break;
        case 12:
            return &InterruptsController::interrupt_callback_pin_12;
            break;
        case 13:
            return &InterruptsController::interrupt_callback_pin_13;
            break;
        case 14:
            return &InterruptsController::interrupt_callback_pin_14;
            break;
        case 15:
            return &InterruptsController::interrupt_callback_pin_15;
            break;
        case 16:
            return &InterruptsController::interrupt_callback_pin_16;
            break;
        case 17:
            return &InterruptsController::interrupt_callback_pin_17;
            break;
        case 18:
            return &InterruptsController::interrupt_callback_pin_18;
            break;
        case 19:
            return &InterruptsController::interrupt_callback_pin_19;
            break;
        case 20:
            return &InterruptsController::interrupt_callback_pin_20;
            break;
        case 21:
            return &InterruptsController::interrupt_callback_pin_21;
            break;
        case 22:
            return &InterruptsController::interrupt_callback_pin_22;
            break;
        case 23:
            return &InterruptsController::interrupt_callback_pin_23;
            break;
        case 24:
            return &InterruptsController::interrupt_callback_pin_24;
            break;
        case 25:
            return &InterruptsController::interrupt_callback_pin_25;
            break;
        case 26:
            return &InterruptsController::interrupt_callback_pin_26;
            break;
        case 27:
            return &InterruptsController::interrupt_callback_pin_27;
            break;
        default:
            return nullptr;
    }
}