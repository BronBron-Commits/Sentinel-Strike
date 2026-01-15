#pragma once

#include "f16_state.hpp"

inline F16State f16_initial_state()
{
    F16State s{};
    s.x = 0.0;
    s.y = 1000.0; // altitude
    s.z = 0.0;

    s.vx = 250.0; // forward speed
    s.vy = 0.0;
    s.vz = 0.0;

    s.pitch = 0.0;
    s.yaw   = 0.0;
    s.roll  = 0.0;

    s.throttle_engaged = true;
    s.afterburner = false;

    s.tick = 0;
    return s;
}
