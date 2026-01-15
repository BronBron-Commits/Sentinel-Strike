add_executable(sentinel-strike
    main.cpp
)

target_include_directories(sentinel-strike PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

find_package(SDL2 REQUIRED)
target_link_libraries(sentinel-strike PRIVATE SDL2::SDL2 SDL2::SDL2main GL GLU glut)
