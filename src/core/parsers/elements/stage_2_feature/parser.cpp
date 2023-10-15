#include "parser.hpp"

#include <sstream>


namespace dlplan::core::parsers::elements::stage_2::parser {

std::shared_ptr<const Boolean> parse_boolean(
    Context& context,
    const error_handler_type& error_handler,
    const stage_1::ast::Element& node) {

}

std::shared_ptr<const Numerical> parse_numerical(
    Context& context,
    const error_handler_type& error_handler,
    const stage_1::ast::Element& node) {

}

std::shared_ptr<const Concept> parse_concept(
    Context& context,
    const error_handler_type& error_handler,
    const stage_1::ast::Element& node) {

}

std::shared_ptr<const Role> parse_role(
    Context& context,
    const error_handler_type& error_handler,
    const stage_1::ast::Element& node) {

}

}