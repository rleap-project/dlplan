#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_PARSER_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_PARSER_H_

#include <string>
#include <unordered_map>

#include "ast.h"
#include "tokenizer.h"
#include "types.h"

namespace dlp {
namespace lisp {

class Parser {
private:
    AST parse_ast(Tokens &tokens) const;

public:
    Parser();

    AST parse(const std::string &description) const;
};

}
}

#endif
