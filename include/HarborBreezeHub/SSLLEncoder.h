//
// Created by enlil on 11/27/21.
//

#ifndef HARBORBREEZEHUB_SSLLENCODER_H
#define HARBORBREEZEHUB_SSLLENCODER_H

#include "core.h"

class SSLLEncoder
{
public:
    const unsigned int SHORT_ON, SHORT_OFF, LONG_ON, LONG_OFF, REST;

    SSLLEncoder(unsigned int shortOn, unsigned int shortOff, unsigned int longOn, unsigned int longOff,
                unsigned int rest);
    virtual ~SSLLEncoder();

    Result getTimingsFromBinaryString(const std::string& binaryStr, std::vector<unsigned int>& timings) const;
    Result getTimingsFromDataString(const std::string& dataStr, std::vector<unsigned int>& timings) const;
};

#endif //HARBORBREEZEHUB_SSLLENCODER_H
