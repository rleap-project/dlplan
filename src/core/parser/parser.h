#ifndef DLPLAN_SRC_CORE_PARSER_PARSER_H_
#define DLPLAN_SRC_CORE_PARSER_PARSER_H_

#include <string>

#include "types.h"


namespace dlplan::core {
class VocabularyInfo;
namespace parser {

class Parser {
private:
    /**
     * Parses tokens into an abstract syntax tree.
     */
    Expression_Ptr parse_ast(const VocabularyInfo& vocabulary_info, Tokens &tokens) const;

public:
    Parser();

    /**
     * Parses a textual description into an abstract syntax tree.
     */
    Expression_Ptr parse(const VocabularyInfo& vocabulary_info, const std::string &description) const;
};

}
}

#endif
