#pragma once

#include "missile_state.hpp"
#include "f16_state.hpp"

bool missile_update_kinematics(
    MissileState &missile,
    const F16State &target,
    float dt
);
