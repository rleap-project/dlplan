#include "expression_factory.h"

#include <algorithm>
#include <iostream>

#include "utils.h"
#include "expressions.h"


namespace dlplan::policy::parser {

std::unordered_map<std::string, EXPRESSION_TYPE> ExpressionFactory::m_element_name_to_expression_type = {
    {":boolean_features", BOOLEAN_FEATURES},
    {":numerical_features", NUMERICAL_FEATURES},
    {":policy", POLICY },
    {":rule", RULE},
    {":conditions", CONDITIONS},
    {":effects", EFFECTS},
    {":c_b_pos", C_B_POS},
    {":c_b_neg", C_B_NEG},
    {":c_n_gt", C_N_GT},
    {":c_n_eq", C_N_EQ},
    {":e_b_pos", E_B_POS},
    {":e_b_neg", E_B_NEG},
    {":e_b_bot", E_B_BOT},
    {":e_n_inc", E_N_INC},
    {":e_n_dec", E_N_DEC},
    {":e_n_bot", E_N_BOT}
};

bool ExpressionFactory::exists_element_name(const std::string& name) {
    return ExpressionFactory::m_element_name_to_expression_type.find(name) != ExpressionFactory::m_element_name_to_expression_type.end();
}

EXPRESSION_TYPE ExpressionFactory::element_name_to_expression_type(const std::string &name) {
    auto p = ExpressionFactory::m_element_name_to_expression_type.find(name);
    if (p == ExpressionFactory::m_element_name_to_expression_type.end()) {
        throw std::runtime_error("ExpressionFactory::element_name_to_expression_type - undefined expression name (" + name + ").");
    }
    return p->second;
}

Expression_Ptr ExpressionFactory::make_expression(const std::string &name, std::vector<Expression_Ptr> &&children) {
    if (ExpressionFactory::exists_element_name(name)) {
        EXPRESSION_TYPE expression_type = ExpressionFactory::element_name_to_expression_type(name);
        switch (expression_type)
        {
            case POLICY: {
                return std::make_unique<PolicyExpression>(PolicyExpression(name, std::move(children)));
            }
            case RULE: {
                return std::make_unique<RuleExpression>(RuleExpression(name, std::move(children)));
            }
            case CONDITIONS: {
                return std::make_unique<ConditionsExpression>(ConditionsExpression(name, std::move(children)));
            }
            case EFFECTS: {
                return std::make_unique<EffectsExpression>(EffectsExpression(name, std::move(children)));
            }
            case BOOLEAN_FEATURES: {
                return std::make_unique<BooleansExpression>(BooleansExpression(name, std::move(children)));
            }
            case NUMERICAL_FEATURES: {
                return std::make_unique<NumericalsExpression>(NumericalsExpression(name, std::move(children)));
            }
            case C_B_POS: {
                return std::make_unique<PositiveBooleanConditionExpression>(PositiveBooleanConditionExpression(name, std::move(children)));
            }
            case C_B_NEG: {
                return std::make_unique<NegativeBooleanConditionExpression>(NegativeBooleanConditionExpression(name, std::move(children)));
            }
            case C_N_GT: {
                return std::make_unique<GreaterNumericalConditionExpression>(GreaterNumericalConditionExpression(name, std::move(children)));
            }
            case C_N_EQ: {
                return std::make_unique<EqualNumericalConditionExpression>(EqualNumericalConditionExpression(name, std::move(children)));
            }
            case E_B_POS: {
                return std::make_unique<PositiveBooleanEffectExpression>(PositiveBooleanEffectExpression(name, std::move(children)));
            }
            case E_B_NEG: {
                return std::make_unique<NegativeBooleanEffectExpression>(NegativeBooleanEffectExpression(name, std::move(children)));
            }
            case E_B_BOT: {
                return std::make_unique<UnchangedBooleanEffectExpression>(UnchangedBooleanEffectExpression(name, std::move(children)));
            }
            case E_N_INC: {
                return std::make_unique<IncrementNumericalEffectExpression>(IncrementNumericalEffectExpression(name, std::move(children)));
            }
            case E_N_DEC: {
                return std::make_unique<DecrementNumericalEffectExpression>(DecrementNumericalEffectExpression(name, std::move(children)));
            }
            case E_N_BOT: {
                return std::make_unique<UnchangedNumericalEffectExpression>(UnchangedNumericalEffectExpression(name, std::move(children)));
            }
        }
    } else if (is_number(name)) {
        return std::make_unique<Expression>(Expression(name, std::move(children)));
    }
    throw std::runtime_error("ExpressionFactory::make_expression - No implementation available for (" + name + ").");
}


}
