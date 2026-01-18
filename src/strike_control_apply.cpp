#include "strike_input.hpp"
#include "strike_scenario.hpp"

void apply_control_input(StrikeScenario& s, const ControlInput& in) {
    switch (in.action) {
        case ControlAction::F16_PitchUp:
            s.f16.pitch += in.value;
            break;
        case ControlAction::F16_PitchDown:
            s.f16.pitch -= in.value;
            break;
        case ControlAction::F16_YawLeft:
            s.f16.yaw -= in.value;
            break;
        case ControlAction::F16_YawRight:
            s.f16.yaw += in.value;
            break;
        case ControlAction::F16_RollLeft:
            s.f16.roll -= in.value;
            break;
        case ControlAction::F16_RollRight:
            s.f16.roll += in.value;
            break;
    }
}
