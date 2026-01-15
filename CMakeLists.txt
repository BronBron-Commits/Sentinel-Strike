cmake_minimum_required(VERSION 3.16)
project(sentinel-strike LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Global include path (authoritative)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(external/sentinel-sim-core)
add_subdirectory(src)
