add_executable(experiment_generator experiment_generator.cpp)
target_link_libraries(experiment_generator dlplancore dlplangenerator dlplanstatespace)

add_custom_target(experiment_generator_link ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_BINARY_DIR}/experiments/experiment_generator" "${CMAKE_CURRENT_SOURCE_DIR}/experiment_generator")
