add_subdirectory(gripper)

add_executable(
    novelty_tests
)
target_sources(
    novelty_tests
    PRIVATE
        tuple_index_generator.cpp
)
target_link_libraries(novelty_tests 
    PRIVATE
        dlplannovelty 
        GTest::GTest)

add_test(novelty_gtests novelty_tests)
