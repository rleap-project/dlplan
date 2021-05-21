#include "parser.h"

#include <vector>

#include "tokenizer.h"

namespace dlp {
namespace lisp {


AST Parser::parse_ast(const Tokens &tokens) const {
    for (auto &token : tokens) {
        TYPE t = token.first;
        switch (t) {
            case LBRACKET: {
                break;
            }
            case RBRACKET: {
                break;
            }
            case COMMA: {
                break;
            }
            case ELEMENT: {
                break;
            }
            case FEATURE: {
                break;
            }
            case PREDICATE: {
                break;
            }
        }
    }
    return AST();
}

Parser::Parser() {
}


AST Parser::parse(
    const std::unordered_map<std::string, unsigned> &predicate_name_to_predicate_idx,
    const std::string &description) const {
    return parse_ast(Tokenizer().tokenize(predicate_name_to_predicate_idx, description));
}

}
}
