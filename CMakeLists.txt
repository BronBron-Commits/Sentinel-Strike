cmake_minimum_required(VERSION 3.16)
project(sentinel_strike LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/sentinel-sim-core)
add_subdirectory(src)
