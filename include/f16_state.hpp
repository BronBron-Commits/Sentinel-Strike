#pragma once

#include <cstdint>

// Pure data state for a single F-16 aircraft.
// No behavior. No side effects. Deterministic-friendly.
struct F16State
{
    // Position (fixed-point or deterministic numeric type later)
    double x;
    double y;
    double z;

    // Velocity
    double vx;
    double vy;
    double vz;

    // Orientation (Euler for now; can evolve later)
    double pitch;
    double yaw;
    double roll;

    // Control state
    bool throttle_engaged;
    bool afterburner;

    // Simulation bookkeeping
    uint64_t tick;
};
