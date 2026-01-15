find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/sentinel-sim-core/include
)

# -------------------------------
# strike-render
# -------------------------------
add_executable(strike-render
    strike_render.cpp
)

target_link_libraries(strike-render
    OpenGL::GL
    OpenGL::GLU
    SDL2::SDL2
)

# -------------------------------
# sentinel-strike
# -------------------------------
add_executable(sentinel-strike
    main.cpp
    sam_radar.cpp
    missile_launch.cpp
    missile_kinematics.cpp
    missile_guidance.cpp
)

target_link_libraries(sentinel-strike
    simcore
)
