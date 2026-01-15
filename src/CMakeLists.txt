find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

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
)

target_link_libraries(sentinel-strike
    simcore
)
