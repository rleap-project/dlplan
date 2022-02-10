add_library(dlplanevaluator SHARED)
target_sources(dlplanevaluator
    PRIVATE
        evaluator.cpp
        ../utils/per_index_bitset.cpp)
target_link_libraries(dlplanevaluator dlplancore)

set(PUBLIC_HEADERS_FILES
    include/dlplan/evaluator.h
    include/dlplan/pimpl.h)
set_target_properties(dlplanevaluator PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS_FILES}")
