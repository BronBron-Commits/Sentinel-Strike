#include "sam_radar.hpp"
#include <cmath>

static inline double deg_to_rad(double d) {
    return d * (3.141592653589793 / 180.0);
}

bool sam_update_radar_lock(
    SamState& sam,
    const F16State& target,
    uint64_t tick
) {
    sam.tick = tick;

    if (!sam.radar_enabled) {
        sam.has_lock = false;
        return false;
    }

    // Relative vector
    const double dx = target.x - sam.x;
    const double dy = target.y - sam.y;

    const double range_sq = dx * dx + dy * dy;
    const double range = std::sqrt(range_sq);

    // Range gate
    if (range > sam.radar_range) {
        sam.has_lock = false;
        return false;
    }

    // Bearing (SAM looks straight up +Y by convention)
    const double bearing = std::atan2(dx, dy); // note swapped for +Y forward
    const double half_fov = deg_to_rad(sam.radar_fov_deg * 0.5);

    if (std::fabs(bearing) > half_fov) {
        sam.has_lock = false;
        return false;
    }

    // Lock acquired or maintained
    if (!sam.has_lock) {
        sam.lock_tick = tick;
    }

    sam.has_lock = true;
    return true;
}
