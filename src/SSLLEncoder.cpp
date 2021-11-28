//
// Created by enlil on 11/27/21.
//

#include "SSLLEncoder.h"

SSLLEncoder::SSLLEncoder(unsigned int shortOn, unsigned int shortOff, unsigned int longOn, unsigned int longOff,
                         unsigned int rest)
        : SHORT_ON(shortOn), SHORT_OFF(shortOff), LONG_ON(longOn), LONG_OFF(longOff), REST(rest)
{
}

SSLLEncoder::~SSLLEncoder() = default;

Result SSLLEncoder::getTimingsFromBinaryString(const std::string& binaryStr, std::vector<unsigned int>& timings) const
{
    timings.clear();
    for (size_t i = 0; i < binaryStr.length(); i++)
    {
        if (i < binaryStr.length() - 1 && binaryStr[i] == binaryStr[i + 1])
        {
            if (binaryStr[i] == '1')
            {
                timings.push_back(LONG_ON);            }
            else
            {
                timings.push_back(LONG_OFF);
            }
            i++; // skip next bit
        }
        else
        {
            if (binaryStr[i] == '1')
            {
                timings.push_back(SHORT_ON);            }
            else
            {
                timings.push_back(SHORT_OFF);
            }
        }
    }
    return Result(true);
}

Result SSLLEncoder::getTimingsFromDataString(const std::string& dataStr, std::vector<unsigned int>& timings) const
{
    timings.clear();
    for (size_t i = 0; i < dataStr.length(); i++)
    {
        if (dataStr[i] == 'S' || dataStr[i] == 's')
        {
            if (i % 2 == 0)
            {
                timings.push_back(SHORT_ON);
            }
            else
            {
                timings.push_back(SHORT_OFF);
            }
        }
        else if (dataStr[i] == 'L' || dataStr[i] == 'l')
        {
            if (i % 2 == 0)
            {
                timings.push_back(LONG_ON);
            }
            else
            {
                timings.push_back(LONG_OFF);
            }
        }
        else
        {
            return Result(false, "Data string contains invalid char at index %u (%c)", i, dataStr[i]);
        }
    }
    return Result(true);
}