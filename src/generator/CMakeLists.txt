add_library(dlplangenerator SHARED
    feature_collection.cpp
    feature_generator.cpp
    generator.cpp
    ../utils/countdown_timer.cpp
    ../utils/sha-256.c
    ../utils/MurmurHash3.cpp
    hash_tables/hash_table.cpp
    hash_tables/hash_table_lossless.cpp
    hash_tables/hash_table_sha-256.cpp
    hash_tables/hash_table_murmur.cpp
)
add_library(staticdlplangenerator STATIC
    feature_collection.cpp
    feature_generator.cpp
    generator.cpp
    ../utils/countdown_timer.cpp
    ../utils/sha-256.c
    ../utils/MurmurHash3.cpp
    hash_tables/hash_table.cpp
    hash_tables/hash_table_lossless.cpp
    hash_tables/hash_table_sha-256.cpp
    hash_tables/hash_table_murmur.cpp
)
set_target_properties(dlplangenerator PROPERTIES PUBLIC_HEADER "include/dlplan/generator.h;include/dlplan/pimpl.h")
