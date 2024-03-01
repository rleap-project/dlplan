enable_testing()

find_package(GTest "1.11.0" REQUIRED COMPONENTS GTest Main PATHS ${CMAKE_PREFIX_PATH} NO_DEFAULT_PATH)
# Set result variables
find_package(GTest)

set(TEST_NAME ${CMAKE_PROJECT_NAME}_tests)

add_subdirectory(core)
add_subdirectory(generator)
add_subdirectory(novelty)
add_subdirectory(policy)
add_subdirectory(state_space)
