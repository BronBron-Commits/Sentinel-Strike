#include "strike_scenario.hpp"

#include <cmath>
#include <cstdio>

#include <simcore/sim_update.hpp>
#include <simcore/sim_initial_state.hpp>

#include "f16_initial_state.hpp"
#include "sam_initial_state.hpp"
#include "missile_initial_state.hpp"

#include "sam_radar.hpp"
#include "missile_launch.hpp"
#include "missile_guidance.hpp"
#include "missile_kinematics.hpp"

/* ---- constants ---- */
static constexpr uint64_t LOCK_TICKS_REQUIRED = 30;

static constexpr double   DT           = 1.0;     // sim seconds per tick
static constexpr double   G            = 9.81;    // m/s^2
static constexpr double   BREAK_G      = 5.0;     // 5g evasive turn

static constexpr double   GUIDANCE_DT  = 0.08;
static constexpr float    KIN_DT       = 1.0f;
static constexpr uint64_t MISSILE_SPACING_TICKS = 10;

void StrikeScenario::init() {
    core    = sim_initial_state();
    f16     = f16_initial_state();
    sam     = sam_initial_state();
    for (int i = 0; i < MAX_MISSILES; ++i)
        missiles[i] = missile_initial_state();
}

static inline void rotate_velocity(double& vx, double& vy, double dtheta) {
    const double c = std::cos(dtheta);
    const double s = std::sin(dtheta);

    const double nx = vx * c - vy * s;
    const double ny = vx * s + vy * c;

    vx = nx;
    vy = ny;
}

void StrikeScenario::step() {
    sim_update(core);

    /* ---- F-16 straight flight until missile launch ---- */
    bool evasive = false;
    for (int i = 0; i < MAX_MISSILES; ++i)
        evasive |= missiles[i].active;

    const double speed = std::hypot(f16.vx, f16.vy);

    if (evasive && speed > 1e-3) {
        /* ---- constant-G break turn (angular model) ---- */
        const double a_lat  = BREAK_G * G;
        const double omega  = a_lat / speed;       // rad/s
        const double dtheta = omega * DT;

        rotate_velocity(f16.vx, f16.vy, dtheta);
    }

    /* ---- integrate position (speed preserved) ---- */
    f16.x += f16.vx * DT;
    f16.y += f16.vy * DT;
    f16.tick = core.tick;

    if ((core.tick % 20) == 0) {
        std::printf(
            "[f16] tick=%llu pos=(%.1f, %.1f) vel=(%.1f, %.1f)\n",
            (unsigned long long)core.tick,
            f16.x, f16.y,
            f16.vx, f16.vy
        );
    }

    /* ---- SAM radar ---- */
    sam_update_radar_lock(sam, f16, core.tick);

    /* ---- missile launch ---- */
    for (int i = 0; i < MAX_MISSILES; ++i) {
        if (missile_launch_predicate(
                sam,
                missiles[i],
                core.tick - i * MISSILE_SPACING_TICKS,
                LOCK_TICKS_REQUIRED))
        {
            missiles[i].x  = sam.x;
            missiles[i].y  = sam.y;
            missiles[i].vx = 0.0f;
            missiles[i].vy = 20.0f;
        }
    }

    /* ---- missile update ---- */
    for (int i = 0; i < MAX_MISSILES; ++i) {
        if (missiles[i].active) {
            missile_update_guidance(missiles[i], f16, GUIDANCE_DT, core.tick);
            missile_update_kinematics(missiles[i], f16, KIN_DT);
        }
    }
}

StrikeFrame StrikeScenario::snapshot() const {
    StrikeFrame f{};
    f.tick = core.tick;

    f.f16 = f16;
    f.sam = sam;
    for (int i = 0; i < MAX_MISSILES; ++i)
        f.missiles[i] = missiles[i];

    return f;
}
