set(TEST_NAME ${CMAKE_PROJECT_NAME}_tests)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_subdirectory(core)
add_subdirectory(generator)
add_subdirectory(novelty)
add_subdirectory(policy)
add_subdirectory(serialization)
add_subdirectory(state_space)
