#pragma once

#include "missile_state.hpp"
#include "f16_state.hpp"
#include <cstdint>

// Simple proportional pursuit guidance (deterministic)
bool missile_update_guidance(
    MissileState& missile,
    const F16State& target,
    double dt,
    uint64_t tick
);
