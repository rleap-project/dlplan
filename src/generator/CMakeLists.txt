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

set(PUBLIC_HEADER_FILES
    include/dlplan/generator.h
    include/dlplan/pimpl.h)
set_target_properties(dlplangenerator PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADER_FILES}")
