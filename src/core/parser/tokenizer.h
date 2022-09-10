#ifndef DLPLAN_SRC_CORE_PARSER_TOKENIZER_H_
#define DLPLAN_SRC_CORE_PARSER_TOKENIZER_H_

#include <string>

#include "types.h"


namespace dlplan::core::parser {

enum class ElementTokenType {
    COMMA,
    OPENING_PARENTHESIS,
    CLOSING_PARENTHESIS,
    NUMBER,
    // concepts
    C_ALL,
    C_AND,
    C_BOT,
    C_DIFF,
    C_EQUAL,
    C_NOT,
    C_ONE_OF,
    C_OR,
    C_PRIMITIVE,
    C_PROJECTION,
    C_SOME,
    C_SUBSET,
    C_TOP,
    // roles
    R_AND,
    R_COMPOSE,
    R_DIFF,
    R_IDENTITY,
    R_INVERSE,
    R_NOT,
    R_OR,
    R_PRIMITIVE,
    R_RESTRICT,
    R_TOP,
    R_TRANSITIVE_CLOSURE,
    R_TRANSITIVE_REFLEXIVE_CLOSURE,
    // numerical
    N_COUNT,
    N_CONCEPT_DISTANCE,
    N_SUM_CONCEPT_DISTANCE,
    N_ROLE_DISTANCE,
    N_SUM_ROLE_DISTANCE,
    // boolean
    B_EMPTY,
    B_INCLUSION,
    B_NULLARY,
};


class Tokenizer {
public:
    Tokenizer();

    Tokens tokenize(const std::string &description) const;
};


}

#endif
