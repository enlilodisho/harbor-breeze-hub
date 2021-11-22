//
// Created by enlil on 8/15/21.
//

#ifndef HARBORBREEZEHUB_CORE_H
#define HARBORBREEZEHUB_CORE_H

#define IRQ_TIMINGS_PATH                "/sys/class/irq_timings/"
#define IRQ_TIMINGS_GPIO_FILE_PREFIX    "gpio"
#define IRQ_TIMINGS_GPIO_FILE(PIN)      IRQ_TIMINGS_PATH IRQ_TIMINGS_GPIO_FILE_PREFIX #PIN

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "ComponentEventSystem/Component.h"
#include "ComponentEventSystem/Event.h"
#include "ComponentEventSystem/Result.h"

#endif //HARBORBREEZEHUB_CORE_H
