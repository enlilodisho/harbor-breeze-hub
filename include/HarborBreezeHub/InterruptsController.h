//
// Created by enlil on 7/13/21.
//

#ifndef HARBORBREEZEHUB_INTERRUPTSCONTROLLER_H
#define HARBORBREEZEHUB_INTERRUPTSCONTROLLER_H

#include "core.h"

#include <chrono>
#include <functional>
#include <memory>
#include <unordered_map>

class InterruptsController
{
public:
    typedef std::chrono::steady_clock clock;

    static InterruptsController& getInstance()
    {
        if (instance_ == nullptr)
        {
            instance_ = new InterruptsController();
        }
        return *instance_;
    }

    static Result registerInterrupt(int pin, int edgeType, const std::function<void(const clock::time_point&)>& interruptHandler);

private:
    static InterruptsController* instance_;
    typedef void(interrupt_callback_fn)();
    static void interrupt_callback_pin_1();
    static void interrupt_callback_pin_2();
    static void interrupt_callback_pin_3();
    static void interrupt_callback_pin_4();
    static void interrupt_callback_pin_5();
    static void interrupt_callback_pin_6();
    static void interrupt_callback_pin_7();
    static void interrupt_callback_pin_8();
    static void interrupt_callback_pin_9();
    static void interrupt_callback_pin_10();
    static void interrupt_callback_pin_11();
    static void interrupt_callback_pin_12();
    static void interrupt_callback_pin_13();
    static void interrupt_callback_pin_14();
    static void interrupt_callback_pin_15();
    static void interrupt_callback_pin_16();
    static void interrupt_callback_pin_17();
    static void interrupt_callback_pin_18();
    static void interrupt_callback_pin_19();
    static void interrupt_callback_pin_20();
    static void interrupt_callback_pin_21();
    static void interrupt_callback_pin_22();
    static void interrupt_callback_pin_23();
    static void interrupt_callback_pin_24();
    static void interrupt_callback_pin_25();
    static void interrupt_callback_pin_26();
    static void interrupt_callback_pin_27();
    static interrupt_callback_fn* get_interrupt_callback_function_by_pin(int pin);

    InterruptsController();
    ~InterruptsController();

    std::unordered_map<int, std::function<void(const clock::time_point&)>> registeredInterrupts_;
    void interrupt_callback(int pin, const clock::time_point& now);
};

#endif //HARBORBREEZEHUB_INTERRUPTSCONTROLLER_H
