#include "parser.h"

#include <vector>
#include <exception>
#include <iostream>
#include <algorithm>

#include "expressions.h"
#include "tokenizer.h"
#include "expression_factory.h"
#include "tokenizer.h"


namespace dlplan::policy::parser {

/**
 * Parses the canonical AST from the given tokens.
 * Tokens in children are sorted lexicographically.
 */
Expression_Ptr Parser::parse_ast(Tokens &tokens) const {
    if (tokens.empty()) {
        throw std::runtime_error("Parser::parse_ast - Unexpected EOF\n");
    }
    // Consume "(".
    Token token = tokens.front();
    tokens.pop_front();
    // std::cout << token.first << " " << token.second << std::endl;
    if (token.second == "(") {
        std::vector<Expression_Ptr> children;
        while (!tokens.empty() && tokens.front().second != ")") {
            children.emplace_back(parse_ast(tokens));
        }
        // Consume ")".
        if (tokens.empty()) throw std::runtime_error("Parser::parse_ast - Expected ')' is missing.");
        tokens.pop_front();
        if (children.empty()) throw std::runtime_error("Parser::parse_ast - Empty list ().");
        std::string name = children.at(0)->get_name();
        // Construct an expression that can be parsed into an element if the description is correct.
        return ExpressionFactory().make_ast(name, std::move(children));
    } else if (token.second == ")") {
        throw std::runtime_error("Parser::parse_ast - Unexpected ')'");
    } else {
        return std::make_unique<Expression>(Expression(token.second, {}));
    }
}

Parser::Parser() = default;

Expression_Ptr Parser::parse(const std::string& data) const {
    Tokens tokens = Tokenizer().tokenize(data);
    return parse_ast(tokens);
}

}
