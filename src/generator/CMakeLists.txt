add_library(dlplangenerator SHARED)

target_sources(dlplangenerator
    PRIVATE
        feature_generator.cpp
        hash_table.cpp
        generator.cpp
        rules/boolean.cpp
        rules/numerical.cpp
        rules/concept.cpp
        rules/role.cpp
        ../utils/logging.cpp
        ../utils/countdown_timer.cpp
        ../utils/sha-256.c
        ../utils/MurmurHash3.cpp
        ../utils/timer.cpp
        ../utils/system.cpp)
target_link_libraries(dlplangenerator dlplancore pthread)
target_include_directories(dlplangenerator
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

install(
    TARGETS
        dlplangenerator
    LIBRARY DESTINATION
        ${CMAKE_INSTALL_PREFIX}/lib/dlplan)

install(
    DIRECTORY
        ${PROJECT_SOURCE_DIR}/include
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}/include)