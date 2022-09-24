add_executable(
    generator_tests
)
target_sources(
    generator_tests
    PRIVATE
        generator.cpp
)
target_link_libraries(generator_tests dlplancore dlplangenerator gtest_main)

add_custom_target(generator_delivery_domain ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/domain.pddl" "${CMAKE_BINARY_DIR}/tests/generator/delivery/domain.pddl" )
add_custom_target(generator_delivery_fd ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${PROJECT_SOURCE_DIR}/libs/scorpion/fast-downward.py" "${CMAKE_BINARY_DIR}/tests/generator/delivery/fast-downward.py")

gtest_discover_tests(generator_tests)