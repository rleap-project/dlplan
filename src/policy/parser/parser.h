#ifndef DLPLAN_SRC_POLICY_PARSER_PARSER_H_
#define DLPLAN_SRC_POLICY_PARSER_PARSER_H_

#include <string>

#include "types.h"


namespace dlplan::policy::parser {

class Parser {
private:
    /**
     * Parses tokens into an abstract syntax tree.
     */
    Expression_Ptr parse_ast(Tokens &tokens) const;

public:
    Parser();

    /**
     * Parses a textual description into an abstract syntax tree.
     */
    Expression_Ptr parse(const std::string& data) const;
};

}

#endif
