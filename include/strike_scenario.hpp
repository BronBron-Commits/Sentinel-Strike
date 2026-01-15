#pragma once

#include <cstdint>
#include "strike_frame.hpp"

#include "f16_state.hpp"
#include "sam_state.hpp"
#include "missile_state.hpp"

struct StrikeScenario {
    uint64_t tick = 0;

    F16State     f16;
    SamState     sam;
    MissileState missile;

    void init();
    void step();

    StrikeFrame snapshot() const;
};
