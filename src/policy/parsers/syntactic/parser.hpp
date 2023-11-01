#ifndef DLPLAN_SRC_CORE_PARSERS_ELEMENTS_SYNTACTIC_API_HPP_
#define DLPLAN_SRC_CORE_PARSERS_ELEMENTS_SYNTACTIC_API_HPP_

#include "../../../../include/dlplan/policy/parsers/syntactic/ast.hpp"

#include <boost/spirit/home/x3.hpp>


namespace dlplan::policy
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