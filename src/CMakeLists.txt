add_executable(sentinel-strike
    main.cpp
)

target_include_directories(sentinel-strike PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/sentinel-sim-core/include
)

target_link_libraries(sentinel-strike PRIVATE
    simcore
)
