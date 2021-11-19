//
// Created by enlil on 11/18/21.
//

#include "SSLLParser.h"
#include "RFDataReceivedEvent.h"
#include <iostream>

SSLLParser::SSLLParser(const std::string& instanceName, size_t maxMessageLength, unsigned int shortOn,
                       unsigned int shortOff, unsigned int longOn, unsigned int longOff, unsigned int rest)
    : MAX_MESSAGE_LENGTH(maxMessageLength), SHORT_ON(shortOn), SHORT_OFF(shortOff), LONG_ON(longOn), LONG_OFF(longOff),
      REST(rest), Component(instanceName)
{

}

SSLLParser::~SSLLParser()
{

}

void SSLLParser::onEvent(Component* sender, std::shared_ptr<Event> event)
{
    if (event->type() == "RFDataReceivedEvent")
    {
        auto& rfDataReceivedEvent = (RFDataReceivedEvent&)*event;
        doParseTimings(rfDataReceivedEvent.getData());
    }
}

Result SSLLParser::doParseTimings(const std::shared_ptr<const std::vector<unsigned int>>& buffer)
{
    switch (parserState_)
    {
        case AWAIT_PATTERN:
        case AWAIT_PATTERN_SPLIT:
            return awaitPattern(*buffer, 0);
        case VERIFY_TIMINGS:
        case VERIFY_TIMINGS_SPLIT:
            return verifyTimings(*buffer, 0);
        default:
            return Result(false, "Invalid parser state");
    }
}

Result SSLLParser::awaitPattern(const std::vector<unsigned int>& buffer, size_t startBufferIndex)
{
    if (parserState_ == AWAIT_PATTERN_SPLIT)
    {
        // if here, then we are continuing to look for the second REST symbol to find a repeated pattern
        bool onSymbol = messageTimings_.size() % 2 == 0;
        for (size_t i = startBufferIndex; i < buffer.size(); i++)
        {
            if (messageTimings_.size() + i > MAX_MESSAGE_LENGTH)
            {
                parserState_ = AWAIT_PATTERN;
                messageTimings_.clear();
                break;
            }
            else if (isValidTiming(buffer[i], REST))
            {
                messageLength_ = messageTimings_.size() + i;
                parserState_ = VERIFY_TIMINGS;

                onSymbol = true;
                for (size_t v = startBufferIndex; v < i; v++)
                {
                    messageTimings_.push_back(getTimingSymbol(buffer[v], onSymbol));
                    onSymbol = !onSymbol;
                }
                startBufferIndex = i + 1;
                // fallback to verify timings
                return verifyTimings(buffer, startBufferIndex);
            }
            else if (getTimingSymbol(buffer[i], onSymbol) != 0)
            {
                onSymbol = !onSymbol;
            }
            else
            {
                parserState_ = AWAIT_PATTERN;
                messageTimings_.clear();
                startBufferIndex = i + 1;
                break;
            }
        }
    }

    if (parserState_ == AWAIT_PATTERN)
    {
        // if here, then we are searching for REST symbols to find a repeated pattern
        ssize_t lastRestIndex = -1;
        bool onSymbol = true;
        // go through the buffer
        for (size_t i = startBufferIndex; i < buffer.size(); i++)
        {
            if (isValidTiming(buffer[i], REST))
            {
                if (lastRestIndex == -1)
                {
                    // found first potential REST symbol
                    lastRestIndex = i;
                }
                else
                {
                    // found second potential REST symbol
                    size_t distance = i - (lastRestIndex + 1); // number of symbols between this and previous REST
                    if (distance <= MAX_MESSAGE_LENGTH)
                    {
                        // We found 2 RESTS. We compute the number of timings between these RESTs to find the message
                        // length.
                        messageLength_ = distance;
                        parserState_ = VERIFY_TIMINGS;
                        break;
                    }
                    else
                    {
                        // The distance between the previous REST and this REST was too great.
                        // Thus the last REST is ignored and we assume that this is the first REST we have received.
                        // Now we will resume looking for another REST.
                        lastRestIndex = i;
                    }
                }
            }
            else if (lastRestIndex != -1)
            {
                if (getTimingSymbol(buffer[i], onSymbol) != 0)
                {
                    onSymbol = !onSymbol;

                    if (i == buffer.size() - 1)
                    {
                        // The end of the buffer has been reached and we have not found 2 REST symbols.
                        if ((i - lastRestIndex) <= MAX_MESSAGE_LENGTH)
                        {
                            // We found 1 REST symbol. There is a chance that the remaining of the message will be found in the
                            // second buffer we receive.
                            parserState_ = AWAIT_PATTERN_SPLIT;
                        }
                    }
                }
                else
                {
                    lastRestIndex = -1;
                    onSymbol = true;
                }
            }
        }
        if (parserState_ == VERIFY_TIMINGS)
        {
            onSymbol = true;
            for (size_t i = lastRestIndex + 1, c = 0; c < messageLength_; i++, c++)
            {
                messageTimings_.push_back(getTimingSymbol(buffer[i], onSymbol));
                onSymbol = !onSymbol;
            }
            startBufferIndex = lastRestIndex + messageLength_ + 2;
            // fallback to verify timings
            return verifyTimings(buffer, startBufferIndex);
        }
        else
        {
            if (parserState_ == AWAIT_PATTERN_SPLIT)
            {
                onSymbol = true;
                for (size_t i = lastRestIndex + 1; i < buffer.size(); i++)
                {
                    messageTimings_.push_back(getTimingSymbol(buffer[i], onSymbol));
                    onSymbol = !onSymbol;
                }
            }
            return Result(true); // finished with this buffer
        }
    }

    return Result(true);
}

Result SSLLParser::verifyTimings(const std::vector<unsigned int>& buffer, size_t startBufferIndex)
{
    if (parserState_ == VERIFY_TIMINGS_SPLIT)
    {
        // if here, then we want to verify we received a repetition of a message that was sent in part in the prior buffer
        for (size_t i = startBufferIndex; i < buffer.size(); i++)
        {
            if (isValidTiming(buffer[i],
                              messageTimings_[verifyMessagePart1Length_ + (i - startBufferIndex)]))
            {
                if (verifyMessagePart1Length_ + i + 1 == messageLength_)
                {
                    // reached end of the message and matched all
                    if (!invokedMessageStart_)
                    {
                        // TODO invoke message received begin event
                        invokedMessageStart_ = true;
                        std::cout << "2 begin message receive\n";
                    }
                    // TODO maybe check if rest is valid?
                    startBufferIndex = i + 2;
                    // fallback to verify timings
                    parserState_ = VERIFY_TIMINGS;
                    break;
                }
            }
            else
            {
                if (invokedMessageStart_)
                {
                    // TODO invoke message received stop event
                    invokedMessageStart_ = false;
                    std::cout << "2 stop message receive\n";
                }
                parserState_ = AWAIT_PATTERN;
                messageTimings_.clear();
                // fallback to await pattern
                return awaitPattern(buffer, startBufferIndex);
            }
        }
    }

    if (parserState_ == VERIFY_TIMINGS)
    {
        // if here, then we want to verify a message is received by checking if timings were sent more than once
        /*std::cout << "received " << messageTimings_.size() << "\n";
        parserState_ = AWAIT_PATTERN;
        messageTimings_.clear();*/
        for (size_t i = startBufferIndex; i < buffer.size(); i++)
        {
            size_t messageTimingsIndex = i - startBufferIndex;
            if (isValidTiming(buffer[i], messageTimings_[messageTimingsIndex]))
            {
                if (messageTimingsIndex == messageTimings_.size() - 1)
                {
                    // if here, then matched with repetition of message
                    if (!invokedMessageStart_)
                    {
                        // TODO invoke message received begin event
                        invokedMessageStart_ = true;
                        std::cout << "1 begin message receive\n";
                    }
                    // TODO maybe check if rest is valid?
                    i++; // skip the next REST
                    startBufferIndex = i + 1;
                }
                else if (i == buffer.size() - 1)
                {
                    // We are currently at last node in buffer but haven't gone through all the message.
                    // Expect the remaining of the message to be in the next buffer.
                    verifyMessagePart1Length_ = buffer.size() - startBufferIndex;
                    parserState_ = VERIFY_TIMINGS_SPLIT;
                    return Result(true); // finished with this buffer
                }
            }
            else
            {
                if (invokedMessageStart_)
                {
                    // TODO invoke message received stop event
                    invokedMessageStart_ = false;
                    std::cout << "1 stop message receive\n";
                }
                parserState_ = AWAIT_PATTERN;
                messageTimings_.clear();
                // fallback to await pattern
                return awaitPattern(buffer, startBufferIndex);
            }
        }
    }

    return Result(true);
}

bool SSLLParser::isValidTiming(const unsigned int& actual, const unsigned int& expected) const
{
    if (actual > expected)
    {
        return (actual - expected) <= errorTolerance_;
    }
    else
    {
        return (expected - actual) <= errorTolerance_;
    }
}

unsigned int SSLLParser::getTimingSymbol(const unsigned int& timing, bool onSymbol) const
{
    if (onSymbol)
    {
        if (isValidTiming(timing, SHORT_ON))
        {
            return SHORT_ON;
        }
        else if (isValidTiming(timing, LONG_ON))
        {
            return LONG_ON;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if (isValidTiming(timing, SHORT_OFF))
        {
            return SHORT_OFF;
        }
        else if (isValidTiming(timing, LONG_OFF))
        {
            return LONG_OFF;
        }
        else
        {
            return 0;
        }
    }
}