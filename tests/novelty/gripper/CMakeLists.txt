add_executable(
    novelty_gripper_tests
)
target_sources(
    novelty_gripper_tests
    PRIVATE
        gripper.cpp
)

add_custom_target(novelty_gripper_domain ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/domain.pddl" "${CMAKE_BINARY_DIR}/tests/novelty/gripper/domain.pddl")
add_custom_target(novelty_gripper_instance ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/p-1-0.pddl" "${CMAKE_BINARY_DIR}/tests/novelty/gripper/p-1-0.pddl")

target_link_libraries(novelty_gripper_tests dlplanstatespace dlplancore dlplannovelty gtest_main)
gtest_discover_tests(novelty_gripper_tests)