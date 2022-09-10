#include "parser.h"

#include <vector>
#include <exception>
#include <iostream>
#include <algorithm>

#include "expressions/expression.h"
#include "expression_factory.h"


namespace dlplan::core::parser {

static TokenRegexes element_token_regexes = {
    { TokenType::COMMA, Tokenizer::build_regex(",") },
    { TokenType::OPENING_PARENTHESIS, Tokenizer::build_regex("\\(") },
    { TokenType::CLOSING_PARENTHESIS, Tokenizer::build_regex("\\)") },
    { TokenType::NAME, Tokenizer::build_regex("[a-zA-Z0-9_\\-]+") },
};


/**
 * Parses the canonical AST from the given tokens.
 * Tokens in children are sorted lexicographically.
 */
Expression_Ptr Parser::parse_ast(const VocabularyInfo& vocabulary_info, Tokens &tokens) const {
    if (tokens.empty()) {
        throw std::runtime_error("Parser::parse_ast - Unexpected EOF\n");
    }
    Token token = tokens.front();
    tokens.pop_front();
    // std::cout << token.first << " " << token.second << std::endl;
    if (!tokens.empty() && tokens.front().first == TokenType::OPENING_PARENTHESIS) {
        // Consume "(".
        tokens.pop_front();
        std::vector<Expression_Ptr> children;
        while (!tokens.empty() && tokens.front().first != TokenType::CLOSING_PARENTHESIS) {
            if (tokens.front().first == TokenType::COMMA) {
                tokens.pop_front();
            }
            children.push_back(parse_ast(vocabulary_info, tokens));
        }
        // Consume ")".
        if (tokens.empty()) throw std::runtime_error("Parser::parse_ast - Expected ')' is missing.");
        tokens.pop_front();
        // Construct an expression that can be parsed into an element if the description is correct.
        return ExpressionFactory().make_ast(vocabulary_info, token.second, std::move(children));
    } else if (token.first == TokenType::CLOSING_PARENTHESIS) {
        throw std::runtime_error("Parser::parse_ast - Unexpected ')'");
    } else {
        return ExpressionFactory().make_ast(vocabulary_info, token.second, {});
    }
}

Parser::Parser() = default;

Expression_Ptr Parser::parse(
    const VocabularyInfo& vocabulary_info,
    const std::string &description) const {
    Tokens tokens = Tokenizer().tokenize(description, element_token_regexes);
    return parse_ast(vocabulary_info, tokens);
}

}
