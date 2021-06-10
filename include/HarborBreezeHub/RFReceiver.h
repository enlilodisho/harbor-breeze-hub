//
// Created by enlil on 4/19/21.
//

#ifndef HARBORBREEZEHUB_RF_RECEIVER_H
#define HARBORBREEZEHUB_RF_RECEIVER_H

#include "ComponentEventSystem/Component.h"
#include "RFProtocol.h"

#include <chrono>
#include <deque>
#include <mutex>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class RFReceiver : public Component
{
private:
    typedef std::chrono::steady_clock clock;

    static const size_t MAX_TIMINGS = 167;
    static const unsigned int MIN_SEPARATION_TIMING;
    static const unsigned int MIN_TIMING;
    static const unsigned int TIMING_MATCH_THRESHOLD;

    static std::unordered_map<int, RFReceiver*> registeredReceivers_;
    static void interrupt_callback(int pin, const clock::time_point& now);
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

    const int pinNumber_;

    clock::time_point last_interrupt_;

    // Contains the protocols the receiver is listening for.
    std::vector<RFProtocol> rfProtocols_ {
            { 350, {  1, 31 }, {  1,  3 }, {  3,  1 }, false },    // protocol 0
            { 650, {  1, 10 }, {  1,  2 }, {  2,  1 }, false },    // protocol 1
            { 100, { 30, 71 }, {  4, 11 }, {  9,  6 }, false },    // protocol 2
            { 380, {  1,  6 }, {  1,  3 }, {  3,  1 }, false },    // protocol 3
            { 500, {  6, 14 }, {  1,  2 }, {  2,  1 }, false },    // protocol 4
            { 450, { 23,  1 }, {  1,  2 }, {  2,  1 }, true },     // protocol 5 (HT6P20B)
            { 150, {  2, 62 }, {  1,  6 }, {  6,  1 }, false },    // protocol 6 (HS2303-PT, i. e. used in AUKEY Remote)
            { 200, {  3, 130}, {  7, 16 }, {  3,  16}, false},     // protocol 7 Conrad RS-200 RX
            { 200, { 130, 7 }, {  16, 7 }, { 16,  3 }, true},      // protocol 8 Conrad RS-200 TX
            { 365, { 18,  1 }, {  3,  1 }, {  1,  3 }, true },     // protocol 9 (1ByOne Doorbell)
            { 270, { 36,  1 }, {  1,  2 }, {  2,  1 }, true },     // protocol 10 (HT12E)
            { 320, { 36,  1 }, {  1,  2 }, {  2,  1 }, true }      // protocol 11 (SM5212)
    };
    // How sensitive are we to delays in receive signal
    int receiveTolerance_ = 120;
    // Timings received (not yet processed)
    std::vector<unsigned int> timings_;
    // How many times this signal has been received
    unsigned int timingsSignalCount_ = 0;
    /*
    std::queue<unsigned long> received_;
    // Mutex to lock queue
    std::mutex received_mutex_;*/

    static inline unsigned int diff(unsigned int a, unsigned int b);
    bool receiveProtocol(const RFProtocol& rfProtocol, unsigned long* outData);
    void handle_interrupt_callback(const clock::time_point& now);

public:
    RFReceiver(std::string instanceName, int pinNumber);
    ~RFReceiver();

    void doWork() override;
    /*
    // Simulates rf receiver received provided data
    void receive(const unsigned long data);*/
    int getPinNumber() const;

    ComponentType type() const override
    {
        return "RFReceiver";
    }
};

#endif //HARBORBREEZEHUB_RF_RECEIVER_H
