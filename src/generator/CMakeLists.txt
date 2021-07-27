add_library(generator
    feature_collection.cpp
    feature_generator.cpp
    generator.cpp
    ../utils/countdown_timer.cpp
    ../utils/sha-256.c
    hash_tables/hash_table.cpp
    hash_tables/hash_table_lossless.cpp
    hash_tables/hash_table_sha-256.cpp
)
set_target_properties(generator PROPERTIES PUBLIC_HEADER "include/dlplan/generator.h;include/dlplan/pimpl.h")
