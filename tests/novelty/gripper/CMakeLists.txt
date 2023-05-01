add_executable(
    novelty_gripper_tests
)
target_sources(
    novelty_gripper_tests
    PRIVATE
        gripper.cpp
)

add_custom_target(novelty_gripper_domain ALL
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/domain.pddl" "${CMAKE_BINARY_DIR}/tests/novelty/gripper/domain.pddl")
add_custom_target(novelty_gripper_instance ALL
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/p-1-0.pddl" "${CMAKE_BINARY_DIR}/tests/novelty/gripper/p-1-0.pddl")

target_link_libraries(novelty_gripper_tests 
    PRIVATE
        dlplanstatespace 
        dlplancore 
        dlplannovelty 
        GTest::GTest)

add_test(novelty_gripper_gtests novelty_gripper_tests)
