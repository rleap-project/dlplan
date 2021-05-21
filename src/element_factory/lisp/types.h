#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_TYPES_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_TYPES_H_

#include <deque>

namespace dlp {
namespace lisp {

enum TYPE {
    NONE,
    LBRACKET,
    RBRACKET,
    STRING,
    INTEGER,
};

using Token = std::pair<TYPE, std::string>;
using Tokens = std::deque<Token>;

}
}


#endif