add_library(dlplannovelty SHARED)
target_sources(dlplannovelty
    PRIVATE
        novelty_base.cpp
        novelty_table.cpp
        novelty.cpp
        tuple_graph_builder.cpp
        tuple_graph.cpp
        tuple_index_generator.cpp
        tuple_node.cpp
        ../utils/math.cpp
    )
target_link_libraries(dlplannovelty dlplancore dlplanstatespace)
target_include_directories(dlplannovelty
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

if (APPLE)
    set_target_properties(dlplannovelty PROPERTIES
        INSTALL_NAME_DIR "@loader_path"
    )
elseif (UNIX)
    set_target_properties(dlplannovelty PROPERTIES
        INSTALL_RPATH "$ORIGIN"
    )
endif()

install(
    TARGETS
        dlplannovelty
    LIBRARY DESTINATION
        ${CMAKE_INSTALL_PREFIX}/lib/dlplan)

install(
    DIRECTORY
        ${PROJECT_SOURCE_DIR}/include/dlplan/
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}/include/dlplan)
