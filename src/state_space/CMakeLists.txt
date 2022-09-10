add_library(dlplanstatespace SHARED)
target_sources(dlplanstatespace
    PRIVATE
        state_space_generator.cpp
        state_space.cpp)
target_link_libraries(dlplanstatespace stdc++fs)
target_include_directories(dlplanstatespace
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

install(
    TARGETS
        dlplanstatespace
    LIBRARY DESTINATION
        ${CMAKE_INSTALL_PREFIX}/lib/dlplan)

install(
    DIRECTORY
        ${PROJECT_SOURCE_DIR}/include/dlplan/
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}/include/dlplan)