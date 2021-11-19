//
// Created by enlil on 11/19/21.
//

#ifndef HARBORBREEZEHUB_SSLLMESSAGEBEGINRECEIVEEVENT_H
#define HARBORBREEZEHUB_SSLLMESSAGEBEGINRECEIVEEVENT_H

#include "core.h"
#include "ComponentEventSystem/Event.h"

class SSLLMessageBeginReceiveEvent : public Event
{
private:
    const std::string binaryStr_;

public:
    explicit SSLLMessageBeginReceiveEvent(const std::string& binaryStr)
        : binaryStr_(binaryStr)
    {
    }
    ~SSLLMessageBeginReceiveEvent() override = default;

    [[nodiscard]] const std::string& getBinaryString() const
    {
        return binaryStr_;
    }

    [[nodiscard]] EventType type() const override
    {
        return "SSLLMessageBeginReceiveEvent";
    }
};

#endif //HARBORBREEZEHUB_SSLLMESSAGEBEGINRECEIVEEVENT_H
