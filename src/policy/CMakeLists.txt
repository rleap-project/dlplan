add_library(policy SHARED)
target_sources(policy
    PRIVATE
        policy.cpp
        policy_impl.cpp
        condition.cpp
        effect.cpp
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
set_target_properties(policy PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS_FILES}")
