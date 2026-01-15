#include <cstdio>
#include <cstdint>
#include <cmath>

#include <simcore/sim_state.hpp>
#include <simcore/sim_update.hpp>
#include <simcore/sim_hash.hpp>
#include <simcore/sim_initial_state.hpp>

#include "f16_state.hpp"
#include "f16_initial_state.hpp"

#include "sam_state.hpp"
#include "sam_initial_state.hpp"
#include "sam_radar.hpp"

#include "missile_state.hpp"
#include "missile_initial_state.hpp"
#include "missile_launch.hpp"
#include "missile_kinematics.hpp"

int main() {
    SimState core = sim_initial_state();

    F16State f16 = f16_initial_state();
    SamState sam = sam_initial_state();
    MissileState missile = missile_initial_state();

    constexpr uint64_t LOCK_TICKS_REQUIRED = 30;

    for (;;) {
        sim_update(core);

        // ---- F-16 derived telemetry (pure)
        const double vx = f16.vx;
        const double vy = f16.vy;
        const double speed = std::sqrt(vx * vx + vy * vy);

        printf("[f16] altitude=%.1f speed=%.1f tick=%llu\n",
               f16.y,
               speed,
               (unsigned long long)core.tick);

        // ---- SAM radar lock update
        if (sam_update_radar_lock(sam, f16, core.tick)) {
            if (sam.lock_tick == core.tick) {
                printf("[sam] lock acquired at tick=%llu\n",
                       (unsigned long long)core.tick);
            }
        }

        // ---- Missile launch predicate
        if (missile_launch_predicate(
        sam,
        missile,
        core.tick,
        LOCK_TICKS_REQUIRED
    )) {
    // launch straight up for now (pure, deterministic)
    missile.x = sam.x;
    missile.y = sam.y;
    missile.vx = 0.0;
    missile.vy = 20.0;

    printf("[missile] launched at tick=%llu\n",
           (unsigned long long)core.tick);
}

        // ---- Missile kinematics update
        missile_update_kinematics(missile);

        if (missile.active) {
            printf("[missile] pos=(%.1f, %.1f) tick=%llu\n",
                   missile.x,
                   missile.y,
                   (unsigned long long)missile.tick);
        }


        // ---- Core hash (verification)
        printf("[core] tick=%llu hash=0x%016llx\n",
               (unsigned long long)core.tick,
               (unsigned long long)sim_hash(core));

        if (core.tick > 120)
            break;
    }

    return 0;
}
