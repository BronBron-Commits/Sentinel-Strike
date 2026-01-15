#pragma once
#include "missile_state.hpp"

inline MissileState missile_initial_state() {
    MissileState m{};
    m.active = false;
    m.detonated = false;
    m.hit = false;
    m.launch_tick = 0;
    m.tick = 0;
    return m;
}
