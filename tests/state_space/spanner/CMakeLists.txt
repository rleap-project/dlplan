add_executable(
    state_space_spanner_tests
)
target_sources(
    state_space_spanner_tests
    PRIVATE
        spanner.cpp
)

add_custom_target(state_space_spanner_domain ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/domain.pddl" "${CMAKE_BINARY_DIR}/tests/state_space/spanner/domain.pddl")
add_custom_target(state_space_spanner_instance ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/problem.pddl" "${CMAKE_BINARY_DIR}/tests/state_space/spanner/problem.pddl")

target_link_libraries(state_space_spanner_tests dlplanstatespace dlplancore gtest_main)
gtest_discover_tests(state_space_spanner_tests)
