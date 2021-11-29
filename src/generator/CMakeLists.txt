add_library(dlplangenerator SHARED
    feature_generator.cpp
    feature_generator_data.cpp
    generator.cpp
    hash_tables/hash_table.cpp
    hash_tables/hash_table_lossless.cpp
    hash_tables/hash_table_sha-256.cpp
    hash_tables/hash_table_murmur.cpp
    ../utils/logging.cpp
    ../utils/countdown_timer.cpp
    ../utils/sha-256.c
    ../utils/MurmurHash3.cpp
    ../utils/timer.cpp
    ../utils/system.cpp
)
add_library(staticdlplangenerator STATIC
    feature_generator.cpp
    feature_generator_data.cpp
    generator.cpp
    hash_tables/hash_table.cpp
    hash_tables/hash_table_lossless.cpp
    hash_tables/hash_table_sha-256.cpp
    hash_tables/hash_table_murmur.cpp
    ../utils/logging.cpp
    ../utils/countdown_timer.cpp
    ../utils/sha-256.c
    ../utils/MurmurHash3.cpp
    ../utils/timer.cpp
    ../utils/system.cpp
)
TARGET_LINK_LIBRARIES(dlplangenerator dlplancore)
TARGET_LINK_LIBRARIES(staticdlplangenerator staticdlplancore)

set_target_properties(dlplangenerator PROPERTIES PUBLIC_HEADER "include/dlplan/generator.h;include/dlplan/pimpl.h")
