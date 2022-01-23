#include "../include/dlplan/policy.h"

#include "condition.h"
#include "effect.h"
#include "rule.h"
#include "policy_impl.h"
#include "policy_builder.h"
#include "reader.h"
#include "writer.h"


namespace dlplan::policy {


PolicyRoot::PolicyRoot() { }

PolicyRoot::~PolicyRoot() { }

BooleanFeature::BooleanFeature(std::shared_ptr<const PolicyRoot> root, int index, core::Boolean&& boolean)
    : Feature<bool>(root, index), m_boolean(std::move(boolean)) { }

bool BooleanFeature::evaluate(const State& state) const {
    return m_boolean.evaluate(*state.get_state());
}

std::string BooleanFeature::compute_repr() const {
    return m_boolean.compute_repr();
}

NumericalFeature::NumericalFeature(std::shared_ptr<const PolicyRoot> root, int index, core::Numerical&& numerical)
    : Feature<int>(root, index), m_numerical(std::move(numerical)) { }

int NumericalFeature::evaluate(const State& state) const {
    return m_numerical.evaluate(*state.get_state());
}

std::string NumericalFeature::compute_repr() const {
    return m_numerical.compute_repr();
}


State::State(int index, std::shared_ptr<const core::State> state)
    : m_index(index), m_state(state) { }

State::~State() { }

int State::get_index() const {
    return m_index;
}

std::shared_ptr<const core::State> State::get_state() const {
    return m_state;
}


std::unique_ptr<BaseCondition> BaseCondition::clone() const {
    return clone_impl();
}


std::unique_ptr<BaseEffect> BaseEffect::clone() const {
    return clone_impl();
}

Rule::Rule(
    std::shared_ptr<const PolicyRoot> root,
    std::unordered_set<std::shared_ptr<const BaseCondition>>&& conditions,
    std::unordered_set<std::shared_ptr<const BaseEffect>>&& effects)
    : m_pImpl(root, std::move(conditions), std::move(effects)) { }

Rule::Rule(const Rule& other)
    : m_pImpl(*other.m_pImpl) { }

Rule& Rule::operator=(const Rule& other) {
    if (this != &other) {
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

Rule::~Rule() { }

bool Rule::evaluate_conditions(const State& source) const {
    return m_pImpl->evaluate_conditions(source);
}

bool Rule::evaluate_effects(const State& source, const State& target) const {
    return m_pImpl->evaluate_effects(source, target);
}

std::string Rule::compute_repr() const {
    return m_pImpl->compute_repr();
}

std::shared_ptr<const PolicyRoot> Rule::get_root() const {
    return m_pImpl->get_root();
}


PolicyBuilder::PolicyBuilder() { }

PolicyBuilder::~PolicyBuilder() { }

std::shared_ptr<const BooleanFeature> PolicyBuilder::add_boolean_feature(core::Boolean boolean) {
    return m_pImpl->add_boolean_feature(boolean);
}

std::shared_ptr<const NumericalFeature> PolicyBuilder::add_numerical_feature(core::Numerical numerical) {
    return m_pImpl->add_numerical_feature(numerical);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_b_pos_condition(std::shared_ptr<const BooleanFeature> b) {
    return m_pImpl->add_b_pos_condition(b);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_b_neg_condition(std::shared_ptr<const BooleanFeature> b) {
    return m_pImpl->add_b_neg_condition(b);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_n_gt_condition(std::shared_ptr<const NumericalFeature> n) {
    return m_pImpl->add_n_gt_condition(n);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_n_eq_condition(std::shared_ptr<const NumericalFeature> n) {
    return m_pImpl->add_n_eq_condition(n);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_b_pos_effect(std::shared_ptr<const BooleanFeature> b) {
    return m_pImpl->add_b_pos_effect(b);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_b_neg_effect(std::shared_ptr<const BooleanFeature> b) {
    return m_pImpl->add_b_neg_effect(b);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_b_bot_effect(std::shared_ptr<const BooleanFeature> b) {
    return m_pImpl->add_b_bot_effect(b);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_n_inc_effect(std::shared_ptr<const NumericalFeature> n) {
    return m_pImpl->add_n_inc_effect(n);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_n_dec_effect(std::shared_ptr<const NumericalFeature> n) {
    return m_pImpl->add_n_dec_effect(n);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_n_bot_effect(std::shared_ptr<const NumericalFeature> n) {
    return m_pImpl->add_n_bot_effect(n);
}

std::shared_ptr<const Rule> PolicyBuilder::add_rule(
    std::unordered_set<std::shared_ptr<const BaseCondition>>&& conditions,
    std::unordered_set<std::shared_ptr<const BaseEffect>>&& effects) {
    return m_pImpl->add_rule(std::move(conditions), std::move(effects));
}

Policy PolicyBuilder::get_result() {
    return m_pImpl->get_result();
}


Policy::Policy(
    std::shared_ptr<const PolicyRoot> root,
    std::vector<std::shared_ptr<const BooleanFeature>>&& boolean_features,
    std::vector<std::shared_ptr<const NumericalFeature>>&& numerical_features,
    std::vector<std::shared_ptr<const Rule>>&& rules)
    : m_pImpl(PolicyImpl(root, std::move(boolean_features), std::move(numerical_features), std::move(rules))) { }

Policy::Policy(const Policy& other) : m_pImpl(*other.m_pImpl) { }

Policy& Policy::operator=(const Policy& other) {
    if (this != &other) {
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

Policy::~Policy() { }

bool Policy::evaluate(const State& source, const State& target) {
    return m_pImpl->evaluate(source, target);
}

std::string Policy::compute_repr() const {
    return m_pImpl->compute_repr();
}

std::shared_ptr<const PolicyRoot> Policy::get_root() const {
    return m_pImpl->get_root();
}

std::vector<std::shared_ptr<const BooleanFeature>> Policy::get_boolean_features() const {
    return m_pImpl->get_boolean_features();
}

std::vector<std::shared_ptr<const NumericalFeature>> Policy::get_numerical_features() const {
    return m_pImpl->get_numerical_features();
}


PolicyReader::PolicyReader() { }

PolicyReader::~PolicyReader() { }

Policy PolicyReader::read(const std::string& data, core::SyntacticElementFactory factory) const {
    return m_pImpl->read(data, factory);
}


PolicyWriter::PolicyWriter() { }

PolicyWriter::~PolicyWriter() { }

std::string PolicyWriter::write(const Policy& policy) const {
    return m_pImpl->write(policy);
}

}
