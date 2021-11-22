//
// Created by enlil on 11/19/21.
//

#ifndef HARBORBREEZEHUB_SSLLMESSAGEBEGINRECEIVEEVENT_H
#define HARBORBREEZEHUB_SSLLMESSAGEBEGINRECEIVEEVENT_H

#include "core.h"

class SSLLMessageBeginReceiveEvent : public Event
{
private:
    const std::string binaryStr_;
    const std::string dataStr_;

public:
    SSLLMessageBeginReceiveEvent(const std::string& binaryStr, const std::string& dataStr)
        : binaryStr_(binaryStr), dataStr_(dataStr)
    {
    }
    ~SSLLMessageBeginReceiveEvent() override = default;

    [[nodiscard]] const std::string& getBinaryString() const
    {
        return binaryStr_;
    }

    [[nodiscard]] const std::string& getDataString() const
    {
        return dataStr_;
    }

    [[nodiscard]] EventType type() const override
    {
        return "SSLLMessageBeginReceiveEvent";
    }
};

#endif //HARBORBREEZEHUB_SSLLMESSAGEBEGINRECEIVEEVENT_H
