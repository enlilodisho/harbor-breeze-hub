//
// Created by enlil on 6/8/21.
//

#ifndef HARBORBREEZEHUB_RFPROTOCOL_H
#define HARBORBREEZEHUB_RFPROTOCOL_H

struct HighLow
{
    uint8_t high;
    uint8_t low;
};

struct RFProtocol
{
    uint16_t pulseLength;

    HighLow syncFactor;
    HighLow zero;
    HighLow one;

    bool invertedSignal;
};

#endif //HARBORBREEZEHUB_RFPROTOCOL_H
