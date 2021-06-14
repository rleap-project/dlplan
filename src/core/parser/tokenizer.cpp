#include "tokenizer.h"

#include <sstream>
#include <iostream>
#include "ast_factory.h"


namespace dlplan::core::parser {

Tokenizer::Tokenizer() = default;

static void add_token(std::stringstream &ss, bool& is_num, Tokens &tokens) {
    std::string token = ss.str();
    if (token.empty()) return;
    if (is_num) {
        tokens.emplace_back(INTEGER, token);
    } else {
        tokens.emplace_back(STRING, token);
    }
    ss.str("");
    ss.clear();
    is_num = true;
}

Tokens Tokenizer::tokenize(const std::string &description) const {
    Tokens tokens;
    std::stringstream ss;
    bool is_num = true;
    for (int i = 0; i < static_cast<int>(description.size()); ++i) {
        char c = tolower(description.at(i));
        if (isdigit(c)) {
            ss << c;
        } else {
            switch (c) {
                case ' ':
                case '\t':
                case ',': {
                    add_token(ss, is_num, tokens);
                    break;
                }
                case '(': {
                    add_token(ss, is_num, tokens);
                    tokens.emplace_back(LBRACKET, "(");
                    break;
                }
                case ')': {
                    add_token(ss, is_num, tokens);
                    tokens.emplace_back(LBRACKET, ")");
                    break;
                }
                default: {
                    ss << c;
                    is_num = false;
                    break;
                }
            }
        }
    }
    add_token(ss, is_num, tokens);
    //for (auto &p : tokens) {
    //    std::cout << p.first << " " << p.second << std::endl;
    //}
    //std::cout << std::endl;
    return tokens;
}

}
