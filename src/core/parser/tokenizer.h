#ifndef DLP_SRC_CORE_PARSER_TOKENIZER_H_
#define DLP_SRC_CORE_PARSER_TOKENIZER_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "types.h"

namespace dlp {
namespace core {
namespace parser {


class Tokenizer {
public:
    Tokenizer();

    Tokens tokenize(const std::string &description) const;
};


}
}
}

#endif
