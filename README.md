
# Sentinel-Strike

**Sentinel-Strike** is a deterministic C++ simulation and visualization project that models a simplified air-defense engagement between an F-16 aircraft, a surface-to-air missile (SAM) site, and an interceptor missile.

The project is designed to demonstrate **correctness-first simulation design**, clean C++ structure, and deterministic replay — not flashy graphics or engine abstractions.

---

## Goals

Sentinel-Strike exists to explore and demonstrate:

- Deterministic simulation using fixed ticks
- Clear separation of **state**, **logic**, and **rendering**
- Reproducible missile–aircraft engagements
- Debug-friendly visualization of simulation state
- Realistic (but intentionally simplified) kinematics

This is a **systems-oriented project**, not a game engine.

---

## Key Features

### Deterministic Core
- Tick-based simulation using `sentinel-sim-core`
- No hidden randomness
- Identical inputs always produce identical outputs
- Designed for replay, rewind, and verification

### Pure State Modeling
All major entities are modeled as pure data:
- `F16State`
- `SamState`
- `MissileState`
- `StrikeFrame` (immutable snapshot per tick)

No rendering or side effects are allowed inside simulation logic.

---

### Missile Engagement Model
- Radar lock with field-of-view and range gating
- Deterministic missile launch predicate
- Boost → sustain acceleration phase
- Proportional pursuit guidance
- Proximity fuse with deterministic hit detection

---

### Aircraft Flight Model
- Straight-and-level flight baseline
- Deterministic constant-G break turn on missile launch
- Angular turn model with bounded acceleration
- Stable velocity magnitude during maneuvering

The intent is **plausibility and stability**, not a full 6-DOF flight model.

---

### Visualization
- OpenGL + SDL renderer
- 3D camera with tilt (not a 2D map view)
- World-space grid for spatial reference
- Trails for aircraft and missile paths
- Step-through playback controls

Rendering is driven **entirely** by recorded simulation frames.

---

## Controls (Renderer)

| Key | Action |
|----|-------|
| `SPACE` | Play / Pause |
| `→` | Step forward one frame |
| `←` | Step backward one frame |
| `R` | Restart playback |
| `ESC` | Exit |

---

## Project Structure

```

sentinel-strike/
├── include/        # Pure state + simulation headers
├── src/
│   ├── strike_scenario.cpp   # Authoritative simulation logic
│   ├── strike_render.cpp     # Deterministic replay renderer
│   ├── missile_*.cpp         # Missile behavior modules
│   └── sam_radar.cpp         # Radar lock logic
├── external/
│   └── sentinel-sim-core     # Deterministic tick core (submodule)
├── CMakeLists.txt
└── README.md

````

---

## Build Instructions

### Requirements
- C++20 compiler
- CMake ≥ 3.16
- SDL2
- OpenGL

### Build
```bash
git clone --recurse-submodules <repo-url>
cd sentinel-strike
mkdir build
cd build
cmake ..
cmake --build . -j
````

### Run

```bash
./src/sentinel-strike     # headless simulation with logging
./src/strike-render       # visual deterministic playback
```

---

## Design Principles

* **Determinism first** — visuals never influence state
* **Explicit time** — no frame-rate coupling
* **Pure functions where possible**
* **Rollback-friendly architecture**
* **Debug visibility over realism theater**

If something is unclear, it should be logged or visualized — not guessed.

---

## Intended Audience

This project is suitable for:

* Early-career C++ engineers building a systems portfolio
* Anyone learning deterministic simulation techniques
* Engineers interested in replay, rollback, or verification-friendly design

It is **not** intended to be:

* A full flight simulator
* A physics-accurate military model
* A general-purpose game engine

---

## Roadmap (Possible Next Steps)

* Missile energy depletion and terminal guidance limits
* Multiple aircraft and missiles
* ECM / break-lock mechanics
* Deterministic network replay
* Unit tests for core predicates

---

## License

MIT
