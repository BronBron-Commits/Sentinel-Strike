#pragma once
#include <cstdint>

struct SamState {
    // world position (fixed)
    double x = 0.0;
    double y = 0.0;

    // radar / sensor state
    bool radar_enabled = true;
    double radar_range = 50000.0;   // meters
    double radar_fov_deg = 120.0;

    // engagement state
    bool has_lock = false;
    uint64_t lock_tick = 0;

    // bookkeeping
    uint64_t tick = 0;
};
