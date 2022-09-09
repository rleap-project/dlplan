add_executable(
    state_space_tests
)
target_sources(
    state_space_tests
    PRIVATE
        state_space.cpp
)
target_link_libraries(state_space_tests dlplanstatespace dlplancore gtest_main)
gtest_discover_tests(state_space_tests)