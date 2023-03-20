add_executable(
    state_space_delivery_example
)
target_sources(
    state_space_delivery_example
    PRIVATE
        delivery.cpp
)

add_custom_target(state_space_delivery_example_domain ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/domain.pddl" "${CMAKE_BINARY_DIR}/examples/state_space/delivery/domain.pddl")
    add_custom_target(state_space_delivery_example_instance_1 ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/instance_2_1_0.pddl" "${CMAKE_BINARY_DIR}/examples/state_space/delivery/instance_2_1_0.pddl")
add_custom_target(state_space_delivery_example_instance_2 ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/instance_2_2_0.pddl" "${CMAKE_BINARY_DIR}/examples/state_space/delivery/instance_2_2_0.pddl")

target_link_libraries(state_space_delivery_example dlplancore dlplangenerator dlplanstatespace)
