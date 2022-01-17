#ifndef DLPLAN_SRC_POLICY_PARSER_TOKENIZER_H_
#define DLPLAN_SRC_POLICY_PARSER_TOKENIZER_H_

#include <istream>

#include "types.h"


namespace dlplan::policy::parser {


class Tokenizer {
public:
    Tokens tokenize(const std::string& data) const;
};


}

#endif
