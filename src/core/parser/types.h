#ifndef DLP_SRC_CORE_PARSER_TYPES_H_
#define DLP_SRC_CORE_PARSER_TYPES_H_

#include <deque>
#include <memory>

namespace dlp {
namespace core {
namespace parser {

class Expression;

enum TYPE {
    NONE,
    LBRACKET,
    RBRACKET,
    STRING,
    INTEGER,
};

using Token = std::pair<TYPE, std::string>;
using Tokens = std::deque<Token>;
using Expression_Ptr = std::unique_ptr<Expression>;

}
}
}


#endif