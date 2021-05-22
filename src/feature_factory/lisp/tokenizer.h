#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_TOKENIZER_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_TOKENIZER_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "types.h"

namespace dlp {
namespace lisp {

class Tokenizer {
public:
    Tokenizer();

    Tokens tokenize(const std::string &description) const;
};

}
}

#endif
