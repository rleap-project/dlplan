#include "parser.h"

#include <vector>
#include <exception>
#include <iostream>
#include <algorithm>

#include "expressions.h"
#include "expression_factory.h"


namespace dlplan::policy::parser {

static TokenRegexes element_token_regexes = {
    { TokenType::COMMA, Tokenizer::build_regex(",") },
    { TokenType::OPENING_PARENTHESIS, Tokenizer::build_regex("\\(") },
    { TokenType::CLOSING_PARENTHESIS, Tokenizer::build_regex("\\)") },
    { TokenType::INTEGER, Tokenizer::build_regex("[0-9]+")},
    { TokenType::STRING, Tokenizer::build_regex("[a-zA-Z0-9_,\\)\\(\\s\\-]+", std::regex_constants::ECMAScript, "^\\s*\"(", ")\"\\s*") },
    { TokenType::NAME, Tokenizer::build_regex(":[a-zA-Z0-9_\\-]+") },
};

/**
 * Parses the canonical AST from the given tokens.
 * Tokens in children are sorted lexicographically.
 */
Expression_Ptr Parser::parse_expressions_tree(Tokens &tokens) const {
    if (tokens.empty()) {
        throw std::runtime_error("Parser::parse_expressions_tree - Unexpected EOF\n");
    }
    // Consume "(".
    Token token = tokens.front();
    tokens.pop_front();
    // std::cout << token.first << " " << token.second << std::endl;
    if (token.second == "(") {
        std::vector<Expression_Ptr> children;
        while (!tokens.empty() && tokens.front().second != ")") {
            children.emplace_back(parse_expressions_tree(tokens));
        }
        // Consume ")".
        if (tokens.empty()) throw std::runtime_error("Parser::parse_expressions_tree - Expected ')' is missing.");
        tokens.pop_front();
        if (children.empty()) throw std::runtime_error("Parser::parse_expressions_tree - Empty list ().");
        std::string name = children.at(0)->get_name();
        // Construct an expression that can be parsed into an element if the description is correct.
        return ExpressionFactory().make_expression(name, std::move(children));
    } else if (token.second == ")") {
        throw std::runtime_error("Parser::parse_expressions_tree - Unexpected ')'");
    } else {
        return std::make_unique<Expression>(Expression(token.second, {}));
    }
}

Parser::Parser() = default;

Expression_Ptr Parser::parse(const std::string& data) const {
    Tokens tokens = Tokenizer().tokenize(data, element_token_regexes);
    return parse_expressions_tree(tokens);
}

}
