#include "tokenizer.h"

#include <regex>
#include <sstream>
#include <iostream>

#include "../elements/concepts/all.h"

#include "../../utils/tokenizer.h"


namespace dlplan::core::parser {

Tokenizer::Tokenizer() = default;

static std::vector<std::pair<ElementTokenType, std::regex>> atom_token_regexes = {
    { ElementTokenType::COMMA, utils::Tokenizer<ElementTokenType>::build_regex(",") },
    { ElementTokenType::OPENING_PARENTHESIS, utils::Tokenizer<ElementTokenType>::build_regex("\\(") },
    { ElementTokenType::CLOSING_PARENTHESIS, utils::Tokenizer<ElementTokenType>::build_regex("\\)") },
    { ElementTokenType::NUMBER, utils::Tokenizer<ElementTokenType>::build_regex("[0-9]+") },
    { ElementTokenType::C_ALL, utils::Tokenizer<ElementTokenType>::build_regex(dlplan::core::element::AllConcept::get_name()) }
};


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
        // char c = tolower(description.at(i));
        char c = description.at(i);
        if (isdigit(c)) {
            ss << c;
        } else {
            switch (c) {
                case ' ':
                case '\t':
                case '\n':
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
