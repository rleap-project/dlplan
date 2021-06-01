
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
)
target_link_libraries(${TEST_NAME} core gtest_main)

include(GoogleTest)
gtest_discover_tests(${TEST_NAME})
