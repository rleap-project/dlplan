#include "../include/dlplan/policy.h"

#include "condition.h"
#include "effect.h"
#include "rule.h"
#include "policy_impl.h"
#include "policy_builder.h"
#include "reader.h"
#include "writer.h"

#include "../../include/dlplan/policy.h"


namespace dlplan::policy {


PolicyRoot::PolicyRoot() { }

PolicyRoot::~PolicyRoot() { }

BooleanFeature::BooleanFeature(std::shared_ptr<const PolicyRoot> root, int index, core::Boolean&& boolean)
    : Feature<bool>(root, index), m_boolean(std::move(boolean)) { }

bool BooleanFeature::evaluate(int state_index, const core::State& state, EvaluationCache& evaluation_caches) const {
    return evaluation_caches.get_boolean_evaluator().evaluate(get_index(), get_boolean(), state_index, state, evaluation_caches.get_element_cache());
}

std::string BooleanFeature::compute_repr() const {
    return m_boolean.compute_repr();
}

const core::Boolean& BooleanFeature::get_boolean() const {
    return m_boolean;
}

NumericalFeature::NumericalFeature(std::shared_ptr<const PolicyRoot> root, int index, core::Numerical&& numerical)
    : Feature<int>(root, index), m_numerical(std::move(numerical)) { }

int NumericalFeature::evaluate(int state_index, const core::State& state, EvaluationCache& evaluation_caches) const {
    return evaluation_caches.get_numerical_cache().evaluate(get_index(), get_numerical(), state_index, state, evaluation_caches.get_element_cache());
}

std::string NumericalFeature::compute_repr() const {
    return m_numerical.compute_repr();
}

const core::Numerical& NumericalFeature::get_numerical() const {
    return m_numerical;
}


std::unique_ptr<BaseCondition> BaseCondition::clone() const {
    return clone_impl();
}

std::shared_ptr<const PolicyRoot> BaseCondition::get_root() const {
    return m_root;
}

std::unique_ptr<BaseEffect> BaseEffect::clone() const {
    return clone_impl();
}

std::shared_ptr<const PolicyRoot> BaseEffect::get_root() const {
    return m_root;
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

bool Rule::evaluate_conditions(int source_index, const core::State& source, EvaluationCache& evaluation_caches) const {
    return m_pImpl->evaluate_conditions(source_index, source, evaluation_caches);
}

bool Rule::evaluate_effects(int source_index, const core::State& source, int target_index, const core::State& target, EvaluationCache& evaluation_caches) const {
    return m_pImpl->evaluate_effects(source_index, source, target_index, target, evaluation_caches);
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

std::shared_ptr<const Rule> Policy::evaluate_lazy(int source_index, const core::State& source, int target_index, const core::State& target) {
    return m_pImpl->evaluate_lazy(source_index, source, target_index, target);
}

std::vector<std::shared_ptr<const Rule>> Policy::evaluate_conditions_eager(int source_index, const core::State& source) {
    return m_pImpl->evaluate_conditions_eager(source_index, source);
}

std::shared_ptr<const Rule> Policy::evaluate_effects_lazy(int source_index, const core::State& source, int target_index, const core::State& target, const std::vector<std::shared_ptr<const Rule>>& rules) {
    return m_pImpl->evaluate_effects_lazy(source_index, source, target_index, target, rules);
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

void Policy::clear_evaluation_cache() {
    m_pImpl->clear_evaluation_cache();
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
