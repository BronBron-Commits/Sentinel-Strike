#pragma once
#include <cstdint>

enum class ControlAction : uint8_t {
    /* aircraft attitude */
    F16_PitchUp,
    F16_PitchDown,
    F16_RollLeft,
    F16_RollRight,

    /* engine throttle */
    ThrottleUp,
    ThrottleDown
};

struct ControlInput {
    uint32_t tick;
    ControlAction action;
    float value;
};
