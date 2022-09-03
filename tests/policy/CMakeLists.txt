add_executable(
    policy_tests
)
target_sources(
    policy_tests
    PRIVATE
        policy_minimizer.cpp
)
target_link_libraries(policy_tests dlplanpolicy gtest_main)
gtest_discover_tests(policy_tests)