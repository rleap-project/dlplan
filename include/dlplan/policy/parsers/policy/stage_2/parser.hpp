#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_POLICY_STAGE_2_PARSER_HPP_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_PARSERS_POLICY_STAGE_2_PARSER_HPP_

#include <tuple>
#include <variant>

#include "include/dlplan/policy.h"

#include "include/dlplan/common/parsers/config.hpp"
#include "include/dlplan/policy/parsers/policy/stage_1/ast.hpp"


namespace dlplan::policy::parsers::policy::stage_2::parser {

extern std::shared_ptr<const Policy> parse(
    const stage_1::ast::Policy& node,
    const dlplan::common::parsers::error_handler_type& error_handler,
    PolicyFactory& context);

}


#endif
