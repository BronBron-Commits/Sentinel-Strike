#include "missile_guidance.hpp"
#include <cmath>

bool missile_update_guidance(
    MissileState& missile,
    const F16State& target,
    double dt,
    uint64_t tick
) {
    if (!missile.active || missile.detonated)
        return false;

    // Vector to target
    const double dx = target.x - missile.x;
    const double dy = target.y - missile.y;
    const double dist = std::sqrt(dx * dx + dy * dy);

    // Hit condition (simple proximity fuse)
    constexpr double HIT_RADIUS = 10.0;
    if (dist < HIT_RADIUS) {
        missile.detonated = true;
        missile.hit = true;
        missile.active = false;
        return true;
    }

    // Normalize direction
    if (dist > 1e-6) {
        const double nx = dx / dist;
        const double ny = dy / dist;

        missile.vx = nx * missile.speed;
        missile.vy = ny * missile.speed;
    }

    // Integrate
    missile.x += missile.vx * dt;
    missile.y += missile.vy * dt;
    missile.tick = tick;

    return false;
}
