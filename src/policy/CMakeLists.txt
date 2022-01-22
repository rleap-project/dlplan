add_library(dlplanpolicy SHARED)
target_sources(dlplanpolicy
    PRIVATE
        policy.cpp
        policy_builder.cpp
        policy_impl.cpp
        condition.cpp
        effect.cpp
        evaluation_caches.cpp
        rule.cpp
        reader.cpp
        writer.cpp
        parser/parser.cpp
        parser/tokenizer.cpp
        parser/expression_factory.cpp
        parser/utils.cpp)

set(PUBLIC_HEADERS_FILES
    include/policy.h
    include/pimpl.h
    include/policy.tpp)
set_target_properties(dlplanpolicy PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS_FILES}")
