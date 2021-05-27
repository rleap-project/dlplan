#include "parser.h"

#include <vector>
#include <exception>
#include <iostream>
#include <algorithm>

#include "tokenizer.h"
#include "ast_factory.h"
#include "tokenizer.h"

namespace dlp {
namespace core {
namespace parser {

/**
 * Parses the canonical AST from the given tokens.
 * Tokens in children are sorted lexicographically.
 */
Expression_Ptr Parser::parse_ast(Tokens &tokens) const {
    if (tokens.empty()) {
        throw std::runtime_error("Unexpected EOF\n");
    }
    Token token = tokens.front();
    tokens.pop_front();
    //std::cout << token.first << " " << token.second << std::endl;
    if (tokens.front().second == "(") {
        // Consume "(".
        tokens.pop_front();
        std::vector<Expression_Ptr> children;
        while (tokens.front().second != ")") {
            children.push_back(parse_ast(tokens));
        }
        // Consume ")".
        tokens.pop_front();
        // Construct an expression that can be parsed into an element if the description is correct.
        return AST_Factory().make_ast(token.second, std::move(children));
    } else if (token.second == ")") {
        throw std::runtime_error("Unexpected ')'");
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
}
