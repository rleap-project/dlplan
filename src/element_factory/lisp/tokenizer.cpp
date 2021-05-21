#include "tokenizer.h"

#include <sstream>
#include <iostream>

namespace dlp {
namespace lisp {

Tokenizer::Tokenizer() {
}

static void add_token(std::stringstream &ss, bool &is_num, Tokens &tokens) {
    if (ss.str().empty()) return;
    if (is_num) {
        tokens.emplace_back(INTEGER, ss.str());
    } else {
        tokens.emplace_back(STRING, ss.str());
    }
    ss.str("");
    ss.clear();
    is_num = true;
}

Tokens Tokenizer::tokenize(const std::string &description) const {
    Tokens tokens;
    std::stringstream ss;
    bool is_num = true;
    for (unsigned i = 0; i < description.size(); ++i) {
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
    for (auto &p : tokens) {
        std::cout << p.first << " " << p.second << std::endl;
    }
    return tokens;
}

}
}
