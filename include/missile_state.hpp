#pragma once
#include <cstdint>

struct MissileState {
    // position
    double x = 0.0;
    double y = 0.0;

    // velocity
    double vx = 0.0;
    double vy = 0.0;

    // lifecycle
    bool active = false;
    bool detonated = false;

    // ownership
    enum class Owner : uint8_t {
        SAM,
        F16
    } owner = Owner::SAM;

    uint64_t launch_tick = 0;
    uint64_t tick = 0;
};
