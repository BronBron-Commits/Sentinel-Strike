#include <cstdio>

#include <simcore/sim_state.hpp>
#include <simcore/sim_update.hpp>
#include <simcore/sim_hash.hpp>
#include <simcore/sim_initial_state.hpp>

#include "f16_initial_state.hpp"

int main()
{
    // Core validation
    SimState sim = sim_initial_state();
    for (int i = 0; i < 5; ++i) {
        sim_update(sim);
        printf("[core] tick=%llu hash=0x%016llx\n",
               (unsigned long long)sim.tick,
               (unsigned long long)sim_hash(sim));
    }

    // F-16 pure state validation
    F16State f16 = f16_initial_state();
    printf("[f16] altitude=%.1f speed=%.1f tick=%llu\n",
           f16.y,
           f16.vx,
           (unsigned long long)f16.tick);

    return 0;
}
