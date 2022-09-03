add_executable(
    policy_tests
)
target_sources(
    policy_tests
    PRIVATE
        policy_builder.cpp
        policy_minimizer.cpp
        utils.cpp
)
target_link_libraries(policy_tests dlplanpolicy gtest_main)
gtest_discover_tests(policy_tests)