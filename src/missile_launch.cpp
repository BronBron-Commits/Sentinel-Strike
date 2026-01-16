#include "missile_launch.hpp"

bool missile_launch_predicate(
    const SamState& sam,
    MissileState& missile,
    uint64_t tick,
    uint64_t required_lock_ticks
) {
    if (missile.active || missile.detonated)
        return false;

    if (!sam.has_lock)
        return false;

    if (tick - sam.lock_tick < required_lock_ticks)
        return false;

    missile.active = true;
    missile.launch_tick = tick;
    missile.tick = tick;

    missile.speed = 20.0f;
    missile.max_speed = 180.0f;
    missile.accel = 6.0f;
    missile.phase = MissilePhase::BOOST;

    return true;
}
