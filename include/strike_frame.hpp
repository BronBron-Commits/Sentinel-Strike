#pragma once

#include <cstdint>

#include "f16_state.hpp"
#include "sam_state.hpp"
#include "missile_state.hpp"

/*
 * StrikeFrame
 * ------------
 * Immutable snapshot of the entire combat-relevant world at a single tick.
 * Used for rendering, replay, rewind, and verification.
 *
 * IMPORTANT:
 * - No logic
 * - No mutation
 * - No rendering
 * - Pure data only
 */
struct StrikeFrame {
    uint64_t tick;

    F16State     f16;
    SamState     sam;
    MissileState missile;
};
