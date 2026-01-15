#include "missile_kinematics.hpp"

void missile_update_kinematics(
    MissileState& missile
) {
    if (!missile.active || missile.detonated)
        return;

    // simple straight-line motion
    missile.x += missile.vx;
    missile.y += missile.vy;

    missile.tick++;
}
