# ---- headless simulation ----
add_executable(sentinel-strike
    main.cpp
    strike_scenario.cpp
    strike_record.cpp
    sam_radar.cpp
    missile_launch.cpp
    missile_guidance.cpp
    missile_kinematics.cpp
)

target_link_libraries(sentinel-strike
    simcore
)

# ---- renderer ----
add_executable(strike-render
    strike_render.cpp
    strike_scenario.cpp
    strike_record.cpp
    sam_radar.cpp
    missile_launch.cpp
    missile_guidance.cpp
    missile_kinematics.cpp
)

target_link_libraries(strike-render
    simcore
    GL
    GLU
    SDL2
)
