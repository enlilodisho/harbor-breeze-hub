//
// Created by enlil on 11/19/21.
//

#ifndef HARBORBREEZEHUB_SSLLMESSAGEENDRECEIVEEVENT_H
#define HARBORBREEZEHUB_SSLLMESSAGEENDRECEIVEEVENT_H

#include "core.h"
#include "SSLLMessageBeginReceiveEvent.h"

class SSLLMessageEndReceiveEvent : public SSLLMessageBeginReceiveEvent
{
public:
    explicit SSLLMessageEndReceiveEvent(const std::string& binaryStr, const std::string& dataStr)
        : SSLLMessageBeginReceiveEvent(binaryStr, dataStr)
    {
    }
    ~SSLLMessageEndReceiveEvent() override = default;

    [[nodiscard]] EventType type() const override
    {
        return "SSLLMessageEndReceiveEvent";
    }
};

#endif //HARBORBREEZEHUB_SSLLMESSAGEENDRECEIVEEVENT_H
