//
// Created by enlil on 11/27/21.
//

#ifndef HARBORBREEZEHUB_FANSTATE_H
#define HARBORBREEZEHUB_FANSTATE_H

#include "core.h"

enum Power
{
    OFF,
    ON
};
enum Rotation
{
    CCW,
    CW
};
struct FanState
{
    Power fanPower_ = OFF;
    uint8_t fanSpeed_ = 0;
    Rotation fanRotation_ = CCW;
    Power lightPower_ = OFF;
    uint8_t lightBrightness_ = 0;
};

#endif //HARBORBREEZEHUB_FANSTATE_H
