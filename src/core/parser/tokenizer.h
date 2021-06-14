#ifndef DLPLAN_SRC_CORE_PARSER_TOKENIZER_H_
#define DLPLAN_SRC_CORE_PARSER_TOKENIZER_H_

#include <string>

#include "types.h"


namespace dlplan::core::parser {


class Tokenizer {
public:
    Tokenizer();

    Tokens tokenize(const std::string &description) const;
};


}

#endif
