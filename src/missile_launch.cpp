#include "missile_launch.hpp"

bool missile_launch_predicate(
    const SamState& sam,
    MissileState& missile,
    uint64_t tick,
    uint64_t required_lock_ticks
) {
    // Already launched or detonated → never relaunch
    if (missile.active || missile.detonated)
        return false;

    // Must have radar lock
    if (!sam.has_lock)
        return false;

    // Lock must be held long enough
    if (tick - sam.lock_tick < required_lock_ticks)
        return false;

    // Launch missile
    missile.active = true;
    missile.launch_tick = tick;
    missile.tick = tick;

    return true;
}
