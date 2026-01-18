#pragma once
#include <cstdint>

enum class ControlAction : uint8_t {
    F16_PitchUp,
    F16_PitchDown,
    F16_YawLeft,
    F16_YawRight
};

struct ControlInput {
    uint32_t tick;
    ControlAction action;
    float value;
};
