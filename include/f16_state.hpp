#pragma once
#include <cstdint>

struct F16State {
    /* position */
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    /* velocity (must be double for rotate_velocity) */
    double vx = 0.0;
    double vy = 0.0;
    double vz = 0.0;

    /* orientation */
    double yaw   = 0.0;
    double pitch = 0.0;
    double roll  = 0.0;

    /* engine */
    float throttle = 0.0f;      /* 0.0 = idle, 1.0 = full */
    bool  throttle_engaged = true;
    bool  afterburner = false;

    /* bookkeeping */
    uint64_t tick = 0;
};
