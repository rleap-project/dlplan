add_executable(
    generator_delivery_tests
)
target_sources(
    generator_delivery_tests
    PRIVATE
        delivery.cpp
)

add_custom_target(generator_delivery_domain ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/domain.pddl" "${CMAKE_BINARY_DIR}/tests/generator/delivery/domain.pddl")
add_custom_target(generator_delivery_instance ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/instance_2_2_0.pddl" "${CMAKE_BINARY_DIR}/tests/generator/delivery/instance_2_2_0.pddl")
add_custom_target(generator_delivery_instance_large ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/instance_4_2_29.pddl" "${CMAKE_BINARY_DIR}/tests/generator/delivery/instance_4_2_29.pddl")

target_link_libraries(generator_delivery_tests dlplancore dlplangenerator dlplanstatespace gtest_main)
gtest_discover_tests(generator_delivery_tests)