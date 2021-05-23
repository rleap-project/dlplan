#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_PARSER_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_PARSER_H_

#include <string>
#include <unordered_map>

#include "types.h"

namespace dlp {
namespace lisp {

class Parser {
private:
    /**
     * Parses tokens into an abstract syntax tree.
     */
    Expression_Ptr parse_ast(Tokens &tokens) const;

public:
    Parser();

    /**
     * Parses a textual description into an abstract syntax tree.
     */
    Expression_Ptr parse(const std::string &description) const;
};

}
}

#endif
