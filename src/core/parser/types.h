#ifndef DLPLAN_SRC_CORE_PARSER_TYPES_H_
#define DLPLAN_SRC_CORE_PARSER_TYPES_H_

#include <memory>


namespace dlplan::core::parser {
class Expression;

using Expression_Ptr = std::unique_ptr<Expression>;

}


#endif