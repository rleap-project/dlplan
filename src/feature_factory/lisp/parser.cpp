#include "parser.h"

#include <vector>
#include <exception>
#include <iostream>
#include <algorithm>

#include "tokenizer.h"

namespace dlp {
namespace lisp {

/**
 * Parses the canonical AST from the given tokens.
 * Tokens in children are sorted lexicographically.
 */
Expression_Ptr Parser::parse_ast(Tokens &tokens) const {
    if (tokens.empty()) {
        std::cout << "Unexpected EOF\n" << std::endl;
        throw std::exception();
    }
    Token token = tokens.front();
    tokens.pop_front();
    std::cout << token.first << " " << token.second << std::endl;
    if (tokens.front().second == "(") {
        // Consume "(".
        tokens.pop_front();
        std::vector<Expression_Ptr> children;
        while (tokens.front().second != ")") {
            children.push_back(parse_ast(tokens));
        }
        // Consume ")".
        tokens.pop_front();
        // Sort children lexicographically to obtain canonical representation.
        std::sort(children.begin(), children.end(),
            [](const Expression_Ptr &l, const Expression_Ptr &r){ return l->name() < r->name(); });
        // TODO(dominik): Ensure that either all children are leafs or inner node by definition of primitive concepts and roles.
        // If we detect primitives, mark them in the AST accordingly.
        return AST_Factory().make_ast(token.second, std::move(children));
        //return AST(token.second, std::move(children));
    } else if (token.second == ")") {
        std::cout << "Unexpected ')'" << std::endl;
        throw std::exception();
    } else {
        return AST_Factory().make_ast(token.second, {});
    }
}

Parser::Parser() {
}

Expression_Ptr Parser::parse(
    const std::string &description) const {
    Tokens tokens = Tokenizer().tokenize(description);
    return parse_ast(tokens);
}

}
}
