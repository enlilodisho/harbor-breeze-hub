//
// Created by enlil on 11/19/21.
//

#ifndef HARBORBREEZEHUB_SSLLMESSAGEENDRECEIVEEVENT_H
#define HARBORBREEZEHUB_SSLLMESSAGEENDRECEIVEEVENT_H

#include "core.h"
#include "ComponentEventSystem/Event.h"

class SSLLMessageEndReceiveEvent : public Event
{
private:
    const std::string binaryStr_;

public:
    explicit SSLLMessageEndReceiveEvent(const std::string& binaryStr)
        : binaryStr_(binaryStr)
    {
    }
    ~SSLLMessageEndReceiveEvent() override = default;

    [[nodiscard]] const std::string& getBinaryString() const
    {
        return binaryStr_;
    }

    [[nodiscard]] EventType type() const override
    {
        return "SSLLMessageEndReceiveEvent";
    }
};

#endif //HARBORBREEZEHUB_SSLLMESSAGEENDRECEIVEEVENT_H
