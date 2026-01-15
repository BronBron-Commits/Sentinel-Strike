#pragma once

#include "missile_state.hpp"

// Fixed-step missile motion (no guidance, no target coupling)
// dt is implicit: one tick = one step
void missile_update_kinematics(
    MissileState& missile
);
