//
// Created by enlil on 4/19/21.
//

#ifndef HARBORBREEZEHUB_RF_RECEIVER_H
#define HARBORBREEZEHUB_RF_RECEIVER_H

#include "ComponentEventSystem/Component.h"

#include <chrono>
#include <string>
#include <unordered_map>

class RFReceiver : public Component
{
private:
    const int pinNumber;

    static std::unordered_map<int, RFReceiver*> registeredReceivers;
    static void interrupt_callback(int pin);
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

    typedef std::chrono::steady_clock clock;
    clock::time_point last_interrupt;

    void handle_interrupt_callback();

public:
    RFReceiver(std::string instanceName, int pinNumber);
    ~RFReceiver();

    void doWork() override;
    int getPinNumber() const;

    ComponentType type() const override
    {
        return "RFReceiver";
    }
};

#endif //HARBORBREEZEHUB_RF_RECEIVER_H
