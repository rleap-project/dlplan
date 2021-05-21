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
AST Parser::parse_ast(Tokens &tokens) const {
    if (tokens.empty()) {
        std::cout << "Unexpected EOF\n" << std::endl;
        throw std::exception();
    }
    Token token = tokens.front();
    tokens.pop_front();
    // std::cout << token.first << " " << token.second << std::endl;
    if (token.second == "(") {
        std::vector<std::unique_ptr<AST>> children;
        while (tokens.front().second != ")") {
            children.push_back(std::make_unique<AST>(parse_ast(tokens)));
        }
        // consume "(".
        tokens.pop_front();
        // sort children lexicographically to obtain canonical representation.
        std::sort(children.begin(), children.end(),
            [](const std::unique_ptr<AST> &l, const std::unique_ptr<AST> &r){ return l->name() < r->name(); });
        // TODO(dominik): Ensure that either all children are leafs or inner node by definition of primitive concepts and roles.
        // If we detect primitives, mark them in the AST accordingly.
        return AST(token.second, std::move(children));
    } else if (token.second == ")") {
        std::cout << "Unexpected ')'" << std::endl;
        throw std::exception();
    } else {
        return AST(token.second, {});
    }
}

Parser::Parser() {
}

AST Parser::parse(
    const std::string &description) const {
    Tokens tokens = Tokenizer().tokenize(description);
    return parse_ast(tokens);
}

}
}
