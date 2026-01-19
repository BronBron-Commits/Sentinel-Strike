# Sentinel-Strike

**Sentinel-Strike** is a deterministic C++ simulation and visualization project modeling a simplified air-defense engagement between an F-16 aircraft, a surface-to-air missile (SAM) site, and interceptor missiles.

The project’s primary goal is to demonstrate **correctness-first simulation design**, **deterministic replay**, and **clean separation of concerns** — with visualization used strictly as a debugging and inspection tool.

This is a **systems-oriented simulation project**, not a game engine.

---

## Goals

Sentinel-Strike exists to explore and demonstrate:

- Deterministic simulation using fixed ticks
- Clear separation of **state**, **logic**, **input**, and **rendering**
- Reproducible missile–aircraft engagements
- Debug-friendly visualization of simulation state
- Plausible (intentionally simplified) kinematics

Visual fidelity is secondary to **stability, inspectability, and correctness**.

---

## Key Features

### Deterministic Core
- Tick-based simulation powered by `sentinel-sim-core`
- No hidden randomness
- Identical inputs always produce identical outputs
- Designed for replay, rewind, and verification

---

### Pure State Modeling
All major entities are modeled as pure data:

- `F16State`
- `SamState`
- `MissileState`
- `StrikeFrame` (immutable snapshot per tick)

Simulation logic contains **no rendering, input, or side effects**.

---

### Missile Engagement Model
- Radar lock with deterministic range and field-of-view gating
- Deterministic missile launch predicates
- Boost → sustain acceleration phases
- Proportional pursuit guidance
- Proximity fuse with deterministic hit detection

The model prioritizes **predictability and debuggability** over real-world fidelity.

---

### Aircraft Flight Model
- Straight-and-level baseline
- Deterministic constant-G maneuvering
- Bounded angular acceleration
- Stable velocity magnitude during maneuvers

This is **not** a full 6-DOF flight simulator by design.

---

## Visualization & Debug Rendering

The renderer is intentionally simple and explicit:

- SDL2 + fixed-function OpenGL
- Deterministic playback of recorded simulation frames
- World-space grid and terrain for spatial reference
- Procedural ground with gentle height variation
- Atmospheric sunset sky background
- Subtle atmospheric dust for depth and scale
- 3D chase-style camera (camera never affects simulation)
- Trails for aircraft and missile paths

Rendering is **entirely driven by recorded state** and has no influence on simulation outcomes.

---

## Interactive Mode

In addition to deterministic playback, Sentinel-Strike includes an **interactive inspection mode**:

- Live stepping of the simulation
- Camera controls decoupled from aircraft physics
- Multiple camera perspectives (chase, side, top, front)
- Aircraft controls applied as deterministic inputs

This mode exists strictly for **inspection and visualization**, not gameplay.

---

## Controls (Interactive Renderer)

### Aircraft Controls
| Key | Action |
|----|-------|
| `W` | Pitch nose up |
| `S` | Pitch nose down |
| `A` | Roll left |
| `D` | Roll right |

### Camera Controls
| Key | Action |
|----|-------|
| `← →` | Rotate camera horizontally |
| `↑ ↓` | Rotate camera vertically |
| `1–5` | Switch camera viewpoints |

## Build Instructions

### Requirements
- C++20 compiler
- CMake ≥ 3.16
- SDL2
- OpenGL (fixed-function pipeline)

### Build
git clone --recurse-submodules <repo-url>
cd sentinel-strike
mkdir build
cd build
cmake ..
cmake --build . -j

Run

./sentinel-strike-interactive   # interactive visualization


---

Design Principles

Determinism first — visuals never influence state

Explicit time — no frame-rate coupling

Pure state and logic

Replay- and rollback-friendly architecture

Debug visibility over realism theater


If something is unclear, it should be logged, visualized, or made explicit — not guessed.


---

Roadmap

Missile energy depletion and terminal guidance limits

Multiple aircraft and missiles

ECM / break-lock mechanics

Deterministic network replay

Formal invariant tests for engagement predicates



---

License

MIT

---

