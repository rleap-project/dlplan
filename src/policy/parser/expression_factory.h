#ifndef DLPLAN_SRC_POLICY_PARSER_EXPRESSION_FACTORY_H_
#define DLPLAN_SRC_POLICY_PARSER_EXPRESSION_FACTORY_H_

#include <unordered_map>
#include <vector>

#include "types.h"


namespace dlplan::policy::parser {

enum EXPRESSION_TYPE {
    BOOLEAN_FEATURES,
    NUMERICAL_FEATURES,
    CONDITIONS,
    EFFECTS,
    C_B_POS,
    C_B_NEG,
    C_N_GT,
    C_N_EQ,
    E_B_POS,
    E_B_NEG,
    E_B_BOT,
    E_N_INC,
    E_N_DEC,
    E_N_BOT,
    RULE,
    POLICY,
};

class ExpressionFactory {
private:
    /**
     * Mappings between element names and enum.
     */
    static std::unordered_map<std::string, EXPRESSION_TYPE> m_element_name_to_expression_type;

    static bool exists_element_name(const std::string& name);
    static EXPRESSION_TYPE element_name_to_expression_type(const std::string &name);

public:
    Expression_Ptr make_expression(const std::string &name, std::vector<Expression_Ptr> &&children);
};

}

#endif
