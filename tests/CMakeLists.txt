
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
        core.cpp
        b_empty.cpp
        b_inclusion.cpp
        b_nullary.cpp
        c_all.cpp
        c_some.cpp
        c_and.cpp
        c_or.cpp
        c_diff.cpp
        c_equal.cpp
        c_not.cpp
        c_primitive.cpp
        c_projection.cpp
        c_bot.cpp
        c_top.cpp
        multi_instance.cpp
        c_one_of.cpp
        c_subset.cpp
        r_and.cpp
        r_or.cpp
        r_diff.cpp
        r_inverse.cpp
        r_identity.cpp
        r_restrict.cpp
        r_not.cpp
        r_primitive.cpp
        r_top.cpp
        r_transitive_closure.cpp
        r_transitive_reflexive_closure.cpp
        r_compose.cpp
        n_concept_distance.cpp
        n_sum_concept_distance.cpp
        n_role_distance.cpp
        n_sum_role_distance.cpp
        n_count.cpp
)
target_link_libraries(${TEST_NAME} dlplancore gtest_main)

include(GoogleTest)
gtest_discover_tests(${TEST_NAME})
