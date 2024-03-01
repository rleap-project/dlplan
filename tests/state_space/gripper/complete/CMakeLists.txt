add_executable(
    state_space_gripper_complete_tests
)
target_sources(
    state_space_gripper_complete_tests
    PRIVATE
        gripper.cpp
)

add_custom_target(state_space_gripper_complete_domain ALL
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/domain.pddl" "${CMAKE_BINARY_DIR}/tests/state_space/gripper/complete/domain.pddl")
add_custom_target(state_space_gripper_complete_instance ALL
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/p-1-0.pddl" "${CMAKE_BINARY_DIR}/tests/state_space/gripper/complete/p-1-0.pddl")

target_link_libraries(state_space_gripper_complete_tests
    PRIVATE
        dlplan::statespace
        GTest::GTest
        GTest::Main)

add_test(state_space_gripper_complete_gtests state_space_gripper_complete_tests)
