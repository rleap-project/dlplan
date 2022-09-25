#ifndef DLPLAN_SRC_POLICY_PARSER_TYPES_H_
#define DLPLAN_SRC_POLICY_PARSER_TYPES_H_

#include <deque>
#include <memory>


namespace dlplan::policy::parser {
class Expression;

using Expression_Ptr = std::unique_ptr<Expression>;

}


#endif