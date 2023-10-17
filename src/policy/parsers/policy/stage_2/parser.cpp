#include "include/dlplan/policy/parsers/policy/stage_2/parser.hpp"

#include <sstream>


namespace dlplan::policy::parsers::policy::stage_2::parser {

std::shared_ptr<const Policy> parse(
    const stage_1::ast::Policy& node,
    const dlplan::common::parsers::error_handler_type& error_handler,
    PolicyFactory& context) {
    std::cout << "function called!" << std::endl;
    return nullptr;
}

}