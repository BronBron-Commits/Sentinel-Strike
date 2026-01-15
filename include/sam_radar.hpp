#pragma once

#include "sam_state.hpp"
#include "f16_state.hpp"

// Pure radar lock predicate + state update
// Returns true if lock is acquired or maintained
bool sam_update_radar_lock(
    SamState& sam,
    const F16State& target,
    uint64_t tick
);
