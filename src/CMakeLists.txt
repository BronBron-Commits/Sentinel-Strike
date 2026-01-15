add_executable(sentinel-strike
    main.cpp
    sam_radar.cpp
    missile_launch.cpp
    missile_kinematics.cpp
)

add_executable(strike-render
    strike_render.cpp
)

target_include_directories(sentinel-strike PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/sentinel-sim-core/include
)

target_include_directories(strike-render PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(sentinel-strike PRIVATE
    simcore
)

target_link_libraries(strike-render PRIVATE
    SDL2
)
