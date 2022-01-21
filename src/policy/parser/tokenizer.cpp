#include "tokenizer.h"

#include <sstream>
#include <iostream>


namespace dlplan::policy::parser {

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

Tokens Tokenizer::tokenize(const std::string& data) const {
    Tokens tokens;
    std::stringstream ss;
    bool is_num = true;
    bool in_str = false;
    for (char c : data) {
        // char c = tolower(description.at(i));
        if (isdigit(c)) {
            ss << c;
        } else {
            switch (c) {
                case '"': {
                    in_str = !in_str;
                    break;
                }
                case ' ':
                case '\t':
                case '\n':
                case ',': {
                    if (in_str) {
                        ss << c;
                    } else {
                        add_token(ss, is_num, tokens);
                    }
                    break;
                }
                case '(': {
                    if (in_str) {
                        ss << c;
                    } else {
                        add_token(ss, is_num, tokens);
                        tokens.emplace_back(LBRACKET, "(");
                    }
                    break;
                }
                case ')': {
                    if (in_str) {
                        ss << c;
                    } else {
                        add_token(ss, is_num, tokens);
                        tokens.emplace_back(LBRACKET, ")");
                    }
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
    /*for (auto &p : tokens) {
        std::cout << p.first << " " << p.second << std::endl;
    }
    std::cout << std::endl;*/
    return tokens;
}

}
