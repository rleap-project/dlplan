#include "../include/dlplan/policy.h"

#include "condition.h"
#include "effect.h"
#include "rule.h"
#include "policy_impl.h"
#include "reader.h"
#include "writer.h"


namespace dlplan::policy {


PolicyRoot::PolicyRoot() { }

PolicyRoot::~PolicyRoot() { }


std::string BaseCondition::compute_repr() const {
    return str();
}

std::unique_ptr<BaseCondition> BaseCondition::clone() const {
    return clone_impl();
}

std::string BaseEffect::compute_repr() const {
    return str();
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

bool Rule::is_condition_satisfied() const {
    return m_pImpl->is_condition_satisfied();
}

bool Rule::is_effect_satisfied() const {
    return m_pImpl->is_effect_satisfied();
}

std::string Rule::str() const {
    return m_pImpl->str();
}

std::string Rule::compute_repr() const {
    return m_pImpl->compute_repr();
}

std::shared_ptr<const PolicyRoot> Rule::get_root() const {
    return m_pImpl->get_root();
}


Policy::Policy() { }

Policy::Policy(const Policy& other) : m_pImpl(*other.m_pImpl) { }

Policy& Policy::operator=(const Policy& other) {
    if (this != &other) {
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

Policy::~Policy() { }

std::shared_ptr<Feature<bool>> Policy::add_boolean_feature() {
    return m_pImpl->add_boolean_feature();
}

std::shared_ptr<Feature<int>> Policy::add_numerical_feature() {
    return m_pImpl->add_numerical_feature();
}

std::shared_ptr<const BaseCondition> Policy::add_b_pos_condition(std::shared_ptr<const Feature<bool>> b) {
    return m_pImpl->add_b_pos_condition(b);
}

std::shared_ptr<const BaseCondition> Policy::add_b_neg_condition(std::shared_ptr<const Feature<bool>> b) {
    return m_pImpl->add_b_neg_condition(b);
}

std::shared_ptr<const BaseCondition> Policy::add_n_gt_condition(std::shared_ptr<const Feature<int>> n) {
    return m_pImpl->add_n_gt_condition(n);
}

std::shared_ptr<const BaseCondition> Policy::add_n_eq_condition(std::shared_ptr<const Feature<int>> n) {
    return m_pImpl->add_n_eq_condition(n);
}

std::shared_ptr<const BaseEffect> Policy::add_b_pos_effect(std::shared_ptr<const Feature<bool>> b) {
    return m_pImpl->add_b_pos_effect(b);
}

std::shared_ptr<const BaseEffect> Policy::add_b_neg_effect(std::shared_ptr<const Feature<bool>> b) {
    return m_pImpl->add_b_neg_effect(b);
}

std::shared_ptr<const BaseEffect> Policy::add_b_bot_effect(std::shared_ptr<const Feature<bool>> b) {
    return m_pImpl->add_b_bot_effect(b);
}

std::shared_ptr<const BaseEffect> Policy::add_n_inc_effect(std::shared_ptr<const Feature<int>> n) {
    return m_pImpl->add_n_inc_effect(n);
}

std::shared_ptr<const BaseEffect> Policy::add_n_dec_effect(std::shared_ptr<const Feature<int>> n) {
    return m_pImpl->add_n_dec_effect(n);
}

std::shared_ptr<const BaseEffect> Policy::add_n_bot_effect(std::shared_ptr<const Feature<int>> n) {
    return m_pImpl->add_n_bot_effect(n);
}

std::shared_ptr<const Rule> Policy::add_rule(
    std::unordered_set<std::shared_ptr<const BaseCondition>>&& conditions,
    std::unordered_set<std::shared_ptr<const BaseEffect>>&& effects) {
    return m_pImpl->add_rule(std::move(conditions), std::move(effects));
}


std::string Policy::str() const {
    return m_pImpl->str();
}

std::shared_ptr<const PolicyRoot> Policy::get_root() const {
    return m_pImpl->get_root();
}

std::vector<std::shared_ptr<Feature<bool>>> Policy::get_boolean_features() const {
    return m_pImpl->get_boolean_features();
}

std::vector<std::shared_ptr<Feature<int>>> Policy::get_numerical_features() const {
    return m_pImpl->get_numerical_features();
}


GeneralPolicyReader::GeneralPolicyReader() { }

GeneralPolicyReader::~GeneralPolicyReader() { }

Policy GeneralPolicyReader::read(const std::string& data) const {
    return m_pImpl->read(data);
}


GeneralPolicyWriter::GeneralPolicyWriter() { }

GeneralPolicyWriter::~GeneralPolicyWriter() { }

std::string GeneralPolicyWriter::write(const Policy& policy) const {
    return m_pImpl->write(policy);
}

}
