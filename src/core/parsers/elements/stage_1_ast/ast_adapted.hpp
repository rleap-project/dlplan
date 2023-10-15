#ifndef SRC_CORE_PARSERS_ELEMENTS_STAGE_1_AST_ADAPTED_HPP_
#define SRC_CORE_PARSERS_ELEMENTS_STAGE_1_AST_ADAPTED_HPP_

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include "ast.hpp"


// We need to tell fusion about our rexpr and rexpr_key_value
// to make them a first-class fusion citizens
BOOST_FUSION_ADAPT_STRUCT(dlplan::core::parsers::elements::stage_1::ast::Element)

#endif
