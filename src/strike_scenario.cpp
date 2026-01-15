#include "strike_scenario.hpp"

#include "f16_initial_state.hpp"
#include "sam_initial_state.hpp"
#include "missile_initial_state.hpp"

#include "sam_radar.hpp"
#include "missile_launch.hpp"
#include "missile_guidance.hpp"
#include "missile_kinematics.hpp"

static constexpr uint64_t LOCK_TICKS_REQUIRED = 30;
static constexpr float   GUIDANCE_GAIN = 0.08f;
static constexpr float   DT = 1.0f;

void StrikeScenario::init() {
    tick    = 0;
    f16     = f16_initial_state();
    sam     = sam_initial_state();
    missile = missile_initial_state();
}

void StrikeScenario::step() {
    sam_update_radar_lock(sam, f16, tick);

    if (missile_launch_predicate(
            sam,
            missile,
            tick,
            LOCK_TICKS_REQUIRED
        )) {
        missile.x  = sam.x;
        missile.y  = sam.y;
        missile.vx = 0.0;
        missile.vy = 20.0;
    }

    if (missile.active) {
        missile_update_guidance(missile, f16, GUIDANCE_GAIN, tick);
        missile_update_kinematics(missile, f16, DT);
    }

    ++tick;
}

StrikeFrame StrikeScenario::snapshot() const {
    StrikeFrame f{};
    f.tick = tick;

    f.sam.x = sam.x;
    f.sam.y = sam.y;

    f.f16.x  = f16.x;
    f.f16.y  = f16.y;
    f.f16.vx = f16.vx;
    f.f16.vy = f16.vy;

    f.missile.active = missile.active;
    f.missile.x  = missile.x;
    f.missile.y  = missile.y;
    f.missile.vx = missile.vx;
    f.missile.vy = missile.vy;

    return f;
}
