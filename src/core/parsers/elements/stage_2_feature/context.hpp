#ifndef SRC_CORE_PARSERS_ELEMENTS_STAGE_2_CONTEXT_HPP_
#define SRC_CORE_PARSERS_ELEMENTS_STAGE_2_CONTEXT_HPP_

#include <map>
#include <memory>
#include <string>

#include "include/dlplan/core.h"


namespace dlplan::core::parsers::elements::stage_2 {
    /// @brief Provides additional context during parsing.
    struct Context {
        SyntacticElementFactory& element_factory;

        Context(SyntacticElementFactory& element_factory);
    };
}

#endif