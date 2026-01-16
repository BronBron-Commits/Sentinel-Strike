#include "strike_scenario.hpp"

#include <simcore/sim_update.hpp>
#include <simcore/sim_initial_state.hpp>

#include "f16_initial_state.hpp"
#include "sam_initial_state.hpp"
#include "missile_initial_state.hpp"

#include "sam_radar.hpp"
#include "missile_launch.hpp"
#include "missile_guidance.hpp"
#include "missile_kinematics.hpp"
#include <cmath>
#include <cstdio>

static constexpr uint64_t LOCK_TICKS_REQUIRED = 30;
static constexpr double   GUIDANCE_DT = 0.08;
static constexpr float    KIN_DT      = 1.0f;

// ---- F-16 evasion (deterministic) ----
static bool   evasive = false;
static double evasion_dir = 1.0; // +1 = right turn


void StrikeScenario::init() {
    core    = sim_initial_state();
    f16     = f16_initial_state();
    sam     = sam_initial_state();
    missile = missile_initial_state();
}

void StrikeScenario::step() {
    sim_update(core);

    /* ---- F-16 straight & level flight ---- */
    static constexpr double F16_DT = 1.0;
    f16.x += f16.vx * F16_DT;
    f16.y += f16.vy * F16_DT;
    f16.tick = core.tick;

    // ---- F-16 constant-G evasive turn ----
    if (evasive) {
        constexpr double G = 9.81;
        constexpr double TURN_G = 5.0; // 5G break
        const double accel = TURN_G * G;

        // perpendicular to velocity (2D)
        const double speed = std::sqrt(f16.vx * f16.vx + f16.vy * f16.vy);
        if (speed > 1e-6) {
            const double nx = -f16.vy / speed;
            const double ny =  f16.vx / speed;

            f16.vx += nx * accel * F16_DT * evasion_dir;
            f16.vy += ny * accel * F16_DT * evasion_dir;
        }
    }

if (core.tick % 20 == 0) { 
    printf("[f16] tick=%llu pos=(%.1f, %.1f) vel=(%.1f, %.1f)\n", 
        (unsigned long long)core.tick, 
        f16.x, f16.y, f16.vx, f16.vy); 
} 



    sam_update_radar_lock(sam, f16, core.tick);

    if (missile_launch_predicate(
            sam,
            missile,
            core.tick,
            LOCK_TICKS_REQUIRED))
    {
        missile.x  = sam.x;
        missile.y  = sam.y;
        missile.vx = 0.0;
        missile.vy = 20.0;

        // ---- F-16 break turn on launch ----
        evasive = true;
        evasion_dir = 1.0; // deterministic right turn
    }

    if (missile.active) {
        missile_update_guidance(missile, f16, GUIDANCE_DT, core.tick);
        missile_update_kinematics(missile, f16, KIN_DT);
    }
}

StrikeFrame StrikeScenario::snapshot() const {
    StrikeFrame f{};
    f.tick = core.tick;

    f.f16.x  = f16.x;
    f.f16.y  = f16.y;
    f.f16.vx = f16.vx;
    f.f16.vy = f16.vy;

    f.sam.x = sam.x;
    f.sam.y = sam.y;

    f.missile.active = missile.active;
    f.missile.x  = missile.x;
    f.missile.y  = missile.y;
    f.missile.vx = missile.vx;
    f.missile.vy = missile.vy;

    return f;
}
