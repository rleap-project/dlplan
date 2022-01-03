add_library(dlplangenerator SHARED)

target_sources(dlplangenerator
    PRIVATE
        feature_generator.cpp
        generator.cpp
        ../utils/logging.cpp
        ../utils/countdown_timer.cpp
        ../utils/sha-256.c
        ../utils/MurmurHash3.cpp
        ../utils/timer.cpp
        ../utils/system.cpp)
target_link_libraries(dlplangenerator dlplancore)

set(PUBLIC_HEADER_FILES
    include/dlplan/generator.h
    include/dlplan/pimpl.h)
set_target_properties(dlplangenerator PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADER_FILES}")
