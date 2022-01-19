#include "effect.h"

#include "../include/dlplan/policy.h"


namespace dlplan::policy {

std::unique_ptr<BaseEffect> PositiveBooleanEffect::clone_impl() const {
    return std::make_unique<PositiveBooleanEffect>(*this);
}

PositiveBooleanEffect::PositiveBooleanEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature)
    : Effect<bool>(root, boolean_feature) {}

bool PositiveBooleanEffect::evaluate(const State&, const State& target) const{
    return get_feature()->evaluate(target);
}

std::string PositiveBooleanEffect::str() const{
    return "(:e_b_pos " + get_feature()->str() + ")";
}


std::unique_ptr<BaseEffect> NegativeBooleanEffect::clone_impl() const{
    return std::make_unique<NegativeBooleanEffect>(*this);
}

NegativeBooleanEffect::NegativeBooleanEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature)
    : Effect<bool>(root, boolean_feature) {}

bool NegativeBooleanEffect::evaluate(const State&, const State& target) const{
    return !get_feature()->evaluate(target);
}

std::string NegativeBooleanEffect::str() const{
    return "(:e_b_neg " + get_feature()->str() + ")";
}


std::unique_ptr<BaseEffect> UnchangedBooleanEffect::clone_impl() const{
    return std::make_unique<UnchangedBooleanEffect>(*this);
}

UnchangedBooleanEffect::UnchangedBooleanEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<bool>> boolean_feature)
    : Effect<bool>(root, boolean_feature) {}

bool UnchangedBooleanEffect::evaluate(const State& source, const State& target) const{
    return get_feature()->evaluate(source) == get_feature()->evaluate(target);
}

std::string UnchangedBooleanEffect::str() const{
    return "(:e_b_bot " + get_feature()->str() + ")";
}


std::unique_ptr<BaseEffect> IncrementNumericalEffect::clone_impl() const{
    return std::make_unique<IncrementNumericalEffect>(*this);
}

IncrementNumericalEffect::IncrementNumericalEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature)
    : Effect<int>(root, numerical_feature) {}

bool IncrementNumericalEffect::evaluate(const State& source, const State& target) const{
    return get_feature()->evaluate(source) < get_feature()->evaluate(target);
}

std::string IncrementNumericalEffect::str() const{
    return "(:e_n_inc " + get_feature()->str() + ")";
}


std::unique_ptr<BaseEffect> DecrementNumericalEffect::clone_impl() const{
    return std::make_unique<DecrementNumericalEffect>(*this);
}

DecrementNumericalEffect::DecrementNumericalEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature)
    : Effect<int>(root, numerical_feature) {}

bool DecrementNumericalEffect::evaluate(const State& source, const State& target) const{
    return get_feature()->evaluate(source) > get_feature()->evaluate(target);
}

std::string DecrementNumericalEffect::str() const{
    return "(:e_n_dec " + get_feature()->str() + ")";
}


std::unique_ptr<BaseEffect> UnchangedNumericalEffect::clone_impl() const{
    return std::make_unique<UnchangedNumericalEffect>(*this);
}

UnchangedNumericalEffect::UnchangedNumericalEffect(std::shared_ptr<const PolicyRoot> root, std::shared_ptr<const Feature<int>> numerical_feature)
    : Effect<int>(root, numerical_feature) {}

bool UnchangedNumericalEffect::evaluate(const State& source, const State& target) const{
    return get_feature()->evaluate(source) == get_feature()->evaluate(target);
}

std::string UnchangedNumericalEffect::str() const{
    return "(:e_n_bot " + get_feature()->str() + ")";
}

}
