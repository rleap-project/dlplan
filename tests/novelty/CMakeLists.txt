add_subdirectory(gripper)

add_executable(
    novelty_tests
)
target_sources(
    novelty_tests
    PRIVATE
        tuple_index_generator.cpp
)
target_link_libraries(novelty_tests dlplannovelty gtest_main)
gtest_discover_tests(novelty_tests)
