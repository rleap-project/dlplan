add_library(dlplanpolicy SHARED)
target_sources(dlplanpolicy
    PRIVATE
        condition.cpp
        effect.cpp
        per_state_bitset.cpp
        policy_builder.cpp
        policy_impl.cpp
        policy.cpp
        reader.cpp
        rule.cpp
        writer.cpp
        parser/expression_factory.cpp
        parser/parser.cpp
        parser/tokenizer.cpp
        parser/utils.cpp)
target_link_libraries(dlplanpolicy dlplancore)

set(PUBLIC_HEADERS_FILES
    include/policy.h
    include/pimpl.h
    include/policy.tpp)
set_target_properties(dlplanpolicy PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS_FILES}")
