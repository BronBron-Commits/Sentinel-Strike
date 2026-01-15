#include "strike_scenario.hpp"
#include "strike_timeline.hpp"

/*
 * Runs the authoritative deterministic core
 * and records frames for rendering.
 */
StrikeTimeline load_recorded_timeline() {
    StrikeScenario scenario;
    scenario.init();

    StrikeTimeline timeline;

    while (scenario.core.tick < 200) {
        scenario.step();
        timeline.push_back(scenario.snapshot());
    }

    return timeline;
}
