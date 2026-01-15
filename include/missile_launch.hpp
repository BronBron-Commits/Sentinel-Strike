#pragma once

#include "sam_state.hpp"
#include "missile_state.hpp"

// Launch missile if SAM lock has been held long enough.
// Returns true ONLY on the tick the missile is launched.
bool missile_launch_predicate(
    const SamState& sam,
    MissileState& missile,
    uint64_t tick,
    uint64_t required_lock_ticks
);
