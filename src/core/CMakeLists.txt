add_library(dlplancore SHARED
    atom.cpp
    state.cpp
    constant.cpp
    core.cpp
    element_factory.cpp
    instance_info.cpp
    vocabulary_info.cpp
    predicate.cpp
    object.cpp
    parser/parser.cpp
    parser/tokenizer.cpp
    parser/ast_factory.cpp
    elements/utils.cpp
    ../utils/logging.cpp
    ../utils/system.cpp
    ../utils/timer.cpp
)
add_library(staticdlplancore STATIC
    atom.cpp
    state.cpp
    constant.cpp
    core.cpp
    element_factory.cpp
    instance_info.cpp
    vocabulary_info.cpp
    predicate.cpp
    object.cpp
    parser/parser.cpp
    parser/tokenizer.cpp
    parser/ast_factory.cpp
    elements/utils.cpp
    ../utils/logging.cpp
    ../utils/system.cpp
    ../utils/timer.cpp
)
set_target_properties(dlplancore PROPERTIES PUBLIC_HEADER "include/dlplan/core.h;include/dlplan/pimpl.h;include/dlplan/types.h")
