
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
    core.cpp
    b_empty.cpp
    c_all.cpp
    c_some.cpp
    c_and.cpp
    c_or.cpp
    c_diff.cpp
    c_not.cpp
    c_primitive.cpp
    c_projection.cpp
    c_bot.cpp
    c_top.cpp
    c_one_of.cpp
    c_subset.cpp
    r_transitive_closure.cpp
    n_concept_distance.cpp
    n_sum_role_distance.cpp
    r_compose.cpp
)
target_link_libraries(${TEST_NAME} dlplancore gtest_main)

include(GoogleTest)
gtest_discover_tests(${TEST_NAME})
