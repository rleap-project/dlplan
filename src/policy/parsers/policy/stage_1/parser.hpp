#ifndef DLPLAN_SRC_CORE_PARSERS_ELEMENTS_STAGE_1_PARSER_API_HPP_
#define DLPLAN_SRC_CORE_PARSERS_ELEMENTS_STAGE_1_PARSER_API_HPP_

#include <boost/spirit/home/x3.hpp>

#include "include/dlplan/policy/parsers/policy/stage_1/ast.hpp"


namespace dlplan::policy::parsers::policy::stage_1
{
    namespace x3 = boost::spirit::x3;

    namespace parser {
        struct PolicyRootClass;

        typedef x3::rule<PolicyRootClass, ast::Policy> policy_root_type;

        BOOST_SPIRIT_DECLARE(policy_root_type)
    }

    parser::policy_root_type const& policy_root();
}

#endif