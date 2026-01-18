find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

cmake_minimum_required(VERSION 3.16)
project(sentinel-strike LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Global include path (authoritative)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(external/sentinel-sim-core)
add_subdirectory(src)



# --- Interactive executable ---

# --- Interactive executable ---




add_executable(sentinel-strike-interactive
    src/strike_interactive.cpp
    src/strike_control_apply.cpp
    src/strike_render_api.cpp

    src/strike_scenario.cpp
    src/strike_record.cpp
    src/missile_kinematics.cpp
    src/missile_guidance.cpp
    src/missile_launch.cpp
    src/sam_radar.cpp
)

target_include_directories(sentinel-strike-interactive PRIVATE
    include
    external/sentinel-sim-core/include
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(sentinel-strike-interactive PRIVATE
    simcore
    ${SDL2_LIBRARIES}
    GL
    GLU
)
