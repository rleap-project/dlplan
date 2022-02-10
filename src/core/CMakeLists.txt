add_library(dlplancore SHARED)

target_sources(dlplancore
    PRIVATE
        atom.cpp
        state.cpp
        constant.cpp
        core.cpp
        element_factory.cpp
        evaluation_caches.cpp
        instance_info.cpp
        vocabulary_info.cpp
        predicate.cpp
        object.cpp
        parser/parser.cpp
        parser/tokenizer.cpp
        parser/ast_factory.cpp
        elements/utils.cpp
        ../utils/logging.cpp
        ../utils/per_index_bitset.cpp
        ../utils/system.cpp
        ../utils/timer.cpp)

set(PUBLIC_HEADERS_FILES
    include/dlplan/core.h
    include/dlplan/core.tpp
    include/dlplan/per_index_array.h
    include/dlplan/per_index_bitset.h
    include/dlplan/pimpl.h
    include/dlplan/segmented_array_vector.h
    include/dlplan/types.h
    )
set_target_properties(dlplancore PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS_FILES}")
