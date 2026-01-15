#include "missile_state.hpp"

/*
 * Deterministic missile kinematics
 * --------------------------------
 * Pure Euler integration.
 * No guidance, no randomness.
 */
void missile_update_kinematics(MissileState& m)
{
    if (!m.active)
        return;

    m.x += m.vx;
    m.y += m.vy;
    m.tick++;
}
