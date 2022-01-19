#include "condition.h"

#include "../include/dlplan/policy.h"


namespace dlplan::policy {


std::unique_ptr<BaseCondition> PositiveBooleanCondition::clone_impl() const {
    return std::make_unique<PositiveBooleanCondition>(*this);
}

PositiveBooleanCondition::PositiveBooleanCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature)
    : Condition<bool>(root, boolean_feature) { }

bool PositiveBooleanCondition::evaluate(const State& state) const {
    return get_feature()->evaluate(state);
}

std::string PositiveBooleanCondition::str() const {
    return "(:c_b_pos " + get_feature()->str() + ")";
}


std::unique_ptr<BaseCondition> NegativeBooleanCondition::clone_impl() const {
    return std::make_unique<NegativeBooleanCondition>(*this);
}

NegativeBooleanCondition::NegativeBooleanCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature)
    : Condition<bool>(root, boolean_feature) { }

bool NegativeBooleanCondition::evaluate(const State& state) const {
    return !get_feature()->evaluate(state);
}

std::string NegativeBooleanCondition::str() const {
    return "(:c_b_neg " + get_feature()->str() + ")";
}


std::unique_ptr<BaseCondition> EqualNumericalCondition::clone_impl() const {
    return std::make_unique<EqualNumericalCondition>(*this);
}

EqualNumericalCondition::EqualNumericalCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature)
    : Condition<int>(root, numerical_feature) { }

bool EqualNumericalCondition::evaluate(const State& state) const {
    return get_feature()->evaluate(state) == 0;
}

std::string EqualNumericalCondition::str() const {
    return "(:c_n_eq " + get_feature()->str() + ")";
}


std::unique_ptr<BaseCondition> GreaterNumericalCondition::clone_impl() const {
    return std::make_unique<GreaterNumericalCondition>(*this);
}

GreaterNumericalCondition::GreaterNumericalCondition(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature)
    : Condition<int>(root, numerical_feature) { }

bool GreaterNumericalCondition::evaluate(const State& state) const {
    return get_feature()->evaluate(state) > 0;
}

std::string GreaterNumericalCondition::str() const {
    return "(:c_n_gt " + get_feature()->str() + ")";
}

}
