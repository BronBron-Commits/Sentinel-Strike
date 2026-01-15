#pragma once

#include <vector>
#include "strike_frame.hpp"

/*
 * StrikeTimeline
 * ----------------
 * Ordered, append-only history of StrikeFrames.
 * Used for rewind, playback, and offline rendering.
 */
using StrikeTimeline = std::vector<StrikeFrame>;
