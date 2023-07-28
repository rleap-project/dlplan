add_executable(
    serialization_gripper_tests
)
target_sources(
    serialization_gripper_tests
    PRIVATE
        gripper.cpp
)
add_custom_target(serialization_gripper_domain ALL
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/domain.pddl" "${CMAKE_BINARY_DIR}/tests/serialization/gripper/domain.pddl")
add_custom_target(serialization_gripper_instance_1 ALL
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/p-1-0.pddl" "${CMAKE_BINARY_DIR}/tests/serialization/gripper/p-1-0.pddl")
    add_custom_target(serialization_gripper_instance_2 ALL
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/p-2-0.pddl" "${CMAKE_BINARY_DIR}/tests/serialization/gripper/p-2-0.pddl")


target_link_libraries(serialization_gripper_tests
    PRIVATE
        dlplanserialization
        dlplanstatespace
        Boost::serialization
        GTest::GTest)

add_test(serialization_gtests serialization_gripper_tests)
