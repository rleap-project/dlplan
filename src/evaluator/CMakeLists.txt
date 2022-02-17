add_library(dlplanevaluator SHARED)
target_sources(dlplanevaluator
    PRIVATE
        evaluator.cpp
        ../utils/per_state_bitset.cpp)
target_link_libraries(dlplanevaluator dlplancore)

set(PUBLIC_HEADERS_FILES
    include/dlplan/evaluator.h
    include/dlplan/per_index_array.h
    include/dlplan/per_index_bitset.h
    include/dlplan/segmented_array_vector.h)
set_target_properties(dlplanevaluator PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS_FILES}")
