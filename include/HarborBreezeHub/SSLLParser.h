//
// Created by enlil on 11/18/21.
//

#ifndef HARBORBREEZEHUB_SSLLPARSER_H
#define HARBORBREEZEHUB_SSLLPARSER_H

#include "core.h"

class SSLLParser : public Component {
private:
    const unsigned int SHORT_ON, SHORT_OFF, LONG_ON, LONG_OFF, REST;
    const size_t MAX_MESSAGE_LENGTH;
    unsigned int errorTolerance_ = 100;
    enum ParserState
    {
        AWAIT_PATTERN,
        AWAIT_PATTERN_SPLIT,
        VERIFY_TIMINGS,
        VERIFY_TIMINGS_SPLIT
    };

    ParserState parserState_ = AWAIT_PATTERN;
    // vars that are used when parsing received data
    ssize_t messageLength_ = 0;
    std::vector<unsigned int> messageTimings_;
    std::string messageBinaryStr_;
    size_t verifyMessagePart1Length_ = 0;
    bool invokedMessageStart_ = false;

    Result doParseTimings(const std::shared_ptr<const std::vector<unsigned int>>& buffer);
    Result awaitPattern(const std::vector<unsigned int>& buffer, size_t startBufferIndex = 0);
    Result verifyTimings(const std::vector<unsigned int>& buffer, size_t startBufferIndex = 0);
    [[nodiscard]] bool isValidTiming(const unsigned int& actual, const unsigned int& expected) const;
    [[nodiscard]] unsigned int getTimingSymbol(const unsigned int& timing, bool onSymbol) const;

protected:
    void onEvent(Component* sender, std::shared_ptr<Event> event) override;

public:
    explicit SSLLParser(const std::string& instanceName, size_t maxMessageLength, unsigned int shortOn,
                        unsigned int shortOff, unsigned int longOn, unsigned int longOff, unsigned int rest);
    ~SSLLParser() override;

    Result getBinaryStringFromTimings(const std::vector<unsigned int>& timings, std::string& binaryStr) const;
    Result getTimingsFromBinaryString(const std::string& binaryStr, std::vector<unsigned int>& timings) const;

    [[nodiscard]] ComponentType type() const override
    {
        return "SSLLParser";
    }
};


#endif //HARBORBREEZEHUB_SSLLPARSER_H
