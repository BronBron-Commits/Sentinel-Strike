#pragma once

#include <cstdint>

#include <simcore/sim_state.hpp>

#include "f16_state.hpp"
#include "sam_state.hpp"
#include "missile_state.hpp"
#include "strike_frame.hpp"

struct StrikeScenario {
    // deterministic core
    SimState core;

    // domain state
    F16State     f16;
    SamState     sam;
    static constexpr int MAX_MISSILES = 4;
    MissileState missiles[MAX_MISSILES];

    void init();
    void step();
    StrikeFrame snapshot() const;
};
