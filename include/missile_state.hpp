#pragma once
#include <cstdint>

enum class MissilePhase : uint8_t {
    BOOST,
    SUSTAIN
};

struct MissileState {
    float x = 0.0f;
    float y = 0.0f;

    float vx = 0.0f;
    float vy = 0.0f;

    float speed = 20.0f;
    float max_speed = 180.0f;
    float accel = 6.0f;

    MissilePhase phase = MissilePhase::BOOST;

    bool active = false;
    bool detonated = false;
    bool hit = false;

    uint64_t launch_tick = 0;
    uint64_t tick = 0;
};
