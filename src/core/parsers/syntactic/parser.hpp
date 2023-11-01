#ifndef DLPLAN_SRC_CORE_PARSERS_SYNTACTIC_PARSER_API_HPP_
#define DLPLAN_SRC_CORE_PARSERS_SYNTACTIC_PARSER_API_HPP_

#include "../../../../include/dlplan/core/parsers/syntactic/ast.hpp"

#include <boost/spirit/home/x3.hpp>


namespace dlplan::core
{
    namespace x3 = boost::spirit::x3;

    namespace parser {
        struct ElementRootClass;
        struct BooleanRootClass;
        struct NumericalRootClass;
        struct ConceptRootClass;
        struct RoleRootClass;

        typedef x3::rule<ElementRootClass, ast::Element> element_root_type;
        typedef x3::rule<BooleanRootClass, ast::Boolean> boolean_root_type;
        typedef x3::rule<NumericalRootClass, ast::Numerical> numerical_root_type;
        typedef x3::rule<ConceptRootClass, ast::Concept> concept_root_type;
        typedef x3::rule<RoleRootClass, ast::Role> role_root_type;

        BOOST_SPIRIT_DECLARE(element_root_type, boolean_root_type, numerical_root_type, concept_root_type, role_root_type)
    }

    parser::element_root_type const& element_root();
    parser::boolean_root_type const& boolean_root();
    parser::numerical_root_type const& numerical_root();
    parser::concept_root_type const& concept_root();
    parser::role_root_type const& role_root();
}

#endif