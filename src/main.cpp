#include <cstdio>

#include <simcore/sim_state.hpp>
#include <simcore/sim_update.hpp>
#include <simcore/sim_hash.hpp>
#include <simcore/sim_initial_state.hpp>

int main()
{
    SimState state = sim_initial_state();

    for (int i = 0; i < 10; ++i) {
        sim_update(state);
        printf("tick=%llu hash=0x%016llx\n",
               (unsigned long long)state.tick,
               (unsigned long long)sim_hash(state));
    }

    return 0;
}
