#include "include/dlplan/core/parsers/elements/stage_2/parser.hpp"

#include <sstream>


namespace dlplan::core::parsers::elements::stage_2::parser {

std::shared_ptr<const Boolean> parse_boolean(
    const stage_1::ast::Boolean& node,
    const error_handler_type& error_handler,
    SyntacticElementFactory& element_factory) {

}

std::shared_ptr<const Numerical> parse_numerical(
    const stage_1::ast::Numerical& node,
    const error_handler_type& error_handler,
    SyntacticElementFactory& element_factory) {

}

std::shared_ptr<const Concept> parse_concept(
    const stage_1::ast::Concept& node,
    const error_handler_type& error_handler,
    SyntacticElementFactory& element_factory) {

}

std::shared_ptr<const Role> parse_role(
    const stage_1::ast::Role& node,
    const error_handler_type& error_handler,
    SyntacticElementFactory& element_factory) {

}

}