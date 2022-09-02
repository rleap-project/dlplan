
set(TEST_NAME ${CMAKE_PROJECT_NAME}_tests)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
    ${TEST_NAME}
)
target_sources(
    ${TEST_NAME}
    PRIVATE
        core/core.cpp
        core/b_empty.cpp
        core/b_inclusion.cpp
        core/b_nullary.cpp
        core/c_all.cpp
        core/c_some.cpp
        core/c_and.cpp
        core/c_or.cpp
        core/c_diff.cpp
        core/c_equal.cpp
        core/c_not.cpp
        core/c_primitive.cpp
        core/c_projection.cpp
        core/c_bot.cpp
        core/c_top.cpp
        core/multi_instance.cpp
        core/c_one_of.cpp
        core/c_subset.cpp
        core/r_and.cpp
        core/r_or.cpp
        core/r_diff.cpp
        core/r_inverse.cpp
        core/r_identity.cpp
        core/r_restrict.cpp
        core/r_not.cpp
        core/r_primitive.cpp
        core/r_top.cpp
        core/r_transitive_closure.cpp
        core/r_transitive_reflexive_closure.cpp
        core/r_compose.cpp
        core/n_concept_distance.cpp
        core/n_sum_concept_distance.cpp
        core/n_role_distance.cpp
        core/n_sum_role_distance.cpp
        core/n_count.cpp
)
target_link_libraries(${TEST_NAME} dlplancore gtest_main)

include(GoogleTest)
gtest_discover_tests(${TEST_NAME})
