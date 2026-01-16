#include "missile_guidance.hpp"
#include <cmath>

static constexpr uint64_t BOOST_TICKS = 25;

bool missile_update_guidance(
    MissileState& missile,
    const F16State& target,
    double dt,
    uint64_t tick
) {
    if (!missile.active || missile.detonated)
        return false;

    missile.tick = tick;

    if (missile.phase == MissilePhase::BOOST) {
        missile.speed += missile.accel;
        if (missile.speed >= missile.max_speed ||
            tick - missile.launch_tick >= BOOST_TICKS) {
            missile.speed = missile.max_speed;
            missile.phase = MissilePhase::SUSTAIN;
        }
    }

    const double dx = target.x - missile.x;
    const double dy = target.y - missile.y;
    const double dist = std::sqrt(dx*dx + dy*dy);

    constexpr double HIT_RADIUS = 10.0;
    if (dist < HIT_RADIUS) {
        missile.detonated = true;
        missile.hit = true;
        missile.active = false;
        return true;
    }

    if (dist > 1e-6) {
        const double nx = dx / dist;
        const double ny = dy / dist;

        missile.vx = nx * missile.speed;
        missile.vy = ny * missile.speed;
    }

    missile.x += missile.vx * dt;
    missile.y += missile.vy * dt;

    return false;
}
