#include "missile_kinematics.hpp"
#include <cmath>
#include <cstdio>

static constexpr float PROXIMITY_RADIUS = 25.0f; // deterministic fuse radius

bool missile_update_kinematics(
    MissileState &missile,
    const F16State &target,
    float dt
) {
    if (!missile.active)
        return false;

    // --- Guidance (existing behavior) ---
    float dx = target.x - missile.x;
    float dy = target.y - missile.y;

    float dist = std::sqrt(dx * dx + dy * dy);
    if (dist > 0.0001f) {
        dx /= dist;
        dy /= dist;
    }

    const float turn_rate = 0.03f;
    missile.vx += dx * turn_rate;
    missile.vy += dy * turn_rate;

    // Normalize velocity (keeps determinism stable)
    float speed = std::sqrt(missile.vx * missile.vx +
                            missile.vy * missile.vy);
    if (speed > 0.0001f) {
        missile.vx = missile.vx / speed * missile.speed;
        missile.vy = missile.vy / speed * missile.speed;
    }

    // --- Integrate position ---
    missile.x += missile.vx * dt;
    missile.y += missile.vy * dt;

    // --- Proximity fuse ---
    float hit_dx = target.x - missile.x;
    float hit_dy = target.y - missile.y;
    float hit_dist = std::sqrt(hit_dx * hit_dx + hit_dy * hit_dy);

    if (hit_dist <= PROXIMITY_RADIUS) {
        missile.active = false;
        missile.hit = true;

        std::printf(
            "[HIT] missile detonated at (%.1f, %.1f), target at (%.1f, %.1f)\n",
            missile.x, missile.y,
            target.x, target.y
        );
        return true;
    }

    return false;
}
