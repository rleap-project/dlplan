add_executable(
    policy_tests
)
target_sources(
    policy_tests
    PRIVATE
        policy_builder.cpp
        policy_minimizer.cpp
        ../utils/domain.cpp
)
target_link_libraries(policy_tests
    PRIVATE
        dlplanpolicy
        GTest::GTest)

add_test(policy_gtests policy_tests)
