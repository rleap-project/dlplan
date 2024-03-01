add_subdirectory(gripper)

add_executable(
    novelty_tests
)
target_sources(
    novelty_tests
    PRIVATE
        novelty_base.cpp
        novelty_table.cpp
        tuple_index_generator.cpp
)
target_link_libraries(novelty_tests
    PRIVATE
        dlplan::novelty
        GTest::GTest
        GTest::Main)

add_test(novelty_gtests novelty_tests)
