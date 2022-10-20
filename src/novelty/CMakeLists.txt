add_library(dlplannovelty SHARED)
target_sources(dlplannovelty
    PRIVATE
        novelty_base.cpp
        novelty_table.cpp
        novelty.cpp
        tuple_graph.cpp
        tuple_index_generator.cpp
        tuple_node.cpp
        ../utils/math.cpp
    )
target_link_libraries(dlplannovelty dlplanstatespace)
target_include_directories(dlplannovelty
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

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
