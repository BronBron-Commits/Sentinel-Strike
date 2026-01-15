#pragma once
#include <cstdint>

struct MissileState {
    float x = 0.0f;
    float y = 0.0f;
    float vx = 0.0f;
    float vy = 0.0f;
    float speed = 20.0f;

    bool active = false;     // flying
    bool detonated = false; // fuse triggered
    bool hit = false;       // hit confirmed

    uint64_t launch_tick = 0; // tick missile was launched
    uint64_t tick = 0;        // current sim tick
};
