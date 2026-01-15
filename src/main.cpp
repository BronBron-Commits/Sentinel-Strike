#include <cstdio>
#include <cstdint>
#include <cmath>

#include "strike_scenario.hpp"

#include <simcore/sim_state.hpp>
#include <simcore/sim_update.hpp>
#include <simcore/sim_hash.hpp>
#include <simcore/sim_initial_state.hpp>

int main() {
    // ---- deterministic core
    SimState core = sim_initial_state();

    // ---- unified scenario (shared with renderer)
    StrikeScenario scenario;
    scenario.init();

    uint64_t last_lock_tick = 0;

    for (;;) {
        // advance deterministic time
        sim_update(core);

        // advance scenario exactly once per tick
        scenario.step();

        const auto &f16     = scenario.f16;
        const auto &sam     = scenario.sam;
        const auto &missile = scenario.missile;

        // ---- F-16 telemetry
        const double speed =
            std::sqrt(f16.vx * f16.vx + f16.vy * f16.vy);

        printf("[f16] altitude=%.1f speed=%.1f tick=%llu\n",
               f16.y,
               speed,
               (unsigned long long)core.tick);

        // ---- SAM lock detection (derived, deterministic)
        if (sam.lock_tick != 0 && sam.lock_tick != last_lock_tick) {
            printf("[sam] lock acquired at tick=%llu\n",
                   (unsigned long long)sam.lock_tick);
            last_lock_tick = sam.lock_tick;
        }

        // ---- missile telemetry
        if (missile.active) {
            printf("[missile] pos=(%.1f, %.1f) vel=(%.1f, %.1f) tick=%llu\n",
                   missile.x, missile.y,
                   missile.vx, missile.vy,
                   (unsigned long long)core.tick);
        }

        // ---- core hash (verification)
        printf("[core] tick=%llu hash=0x%016llx\n",
               (unsigned long long)core.tick,
               (unsigned long long)sim_hash(core));

        if (core.tick > 120)
            break;
    }

    return 0;
}
