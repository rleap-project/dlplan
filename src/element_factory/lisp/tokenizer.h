#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_TOKENIZER_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_TOKENIZER_H_

#include <string>
#include <unordered_map>
#include <vector>

namespace dlp {
namespace lisp {

enum TYPE {
    LBRACKET,
    RBRACKET,
    COMMA,
    ELEMENT,
    FEATURE,
    PREDICATE,
};

using Tokens = std::vector<std::pair<TYPE, std::string>>;

class Tokenizer {
public:
    Tokenizer();

    Tokens tokenize(
        const std::unordered_map<std::string, unsigned> &predicate_name_to_predicate_idx,
        const std::string &description) const;
};

}
}

#endif
