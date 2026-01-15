#pragma once
#include "sam_state.hpp"

inline SamState sam_initial_state() {
    SamState s;
    s.x = 0.0;
    s.y = 0.0;
    s.radar_enabled = true;
    s.has_lock = false;
    s.tick = 0;
    return s;
}
