#include "strike_scenario.hpp"
#include "strike_input.hpp"

#include <algorithm> // std::min, std::max

void apply_control_input(StrikeScenario& scenario, const ControlInput& in)
{
    auto& f16 = scenario.f16;

    switch (in.action) {

        /* ----- THROTTLE ----- */
        case ControlAction::ThrottleUp:
            f16.throttle = std::min(1.0f, f16.throttle + in.value);
            break;

        case ControlAction::ThrottleDown:
            f16.throttle = std::max(0.0f, f16.throttle - in.value);
            break;

        /* ----- ATTITUDE ----- */
        case ControlAction::F16_PitchUp:
            f16.pitch += in.value;
            break;

        case ControlAction::F16_PitchDown:
            f16.pitch -= in.value;
            break;

        case ControlAction::F16_RollLeft:
            f16.roll += in.value;
            break;

        case ControlAction::F16_RollRight:
            f16.roll -= in.value;
            break;

        default:
            break;
    }
}
