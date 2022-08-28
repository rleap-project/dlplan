#include "../include/dlplan/policy.h"

#include "condition.h"
#include "effect.h"
#include "policy_builder.h"
#include "reader.h"
#include "writer.h"

#include "../../include/dlplan/policy.h"


namespace dlplan::policy {


BaseCondition::BaseCondition(std::shared_ptr<const core::BaseElement> base_feature)
    : m_base_feature(base_feature) { }

BaseCondition::BaseCondition(BaseCondition&& other) = default;

BaseCondition& BaseCondition::operator=(BaseCondition&& other) = default;

BaseCondition::~BaseCondition() = default;

std::shared_ptr<const core::BaseElement> BaseCondition::get_base_feature() const {
    return m_base_feature;
}

std::string BaseCondition::str() const {
    return compute_repr();
}


BaseEffect::BaseEffect(std::shared_ptr<const core::BaseElement> base_feature)
    : m_base_feature(base_feature) { }

BaseEffect::BaseEffect(BaseEffect&& other) = default;

BaseEffect& BaseEffect::operator=(BaseEffect&& other) = default;

BaseEffect::~BaseEffect() = default;

std::shared_ptr<const core::BaseElement> BaseEffect::get_base_feature() const {
    return m_base_feature;
}

std::string BaseEffect::str() const {
    return compute_repr();
}


PolicyBuilder::PolicyBuilder() = default;

PolicyBuilder::PolicyBuilder(const PolicyBuilder& other)
    : m_pImpl(*other.m_pImpl) { }

PolicyBuilder& PolicyBuilder::operator=(const PolicyBuilder& other) {
    if (this != &other) {
        *m_pImpl = *other.m_pImpl;
    }
    return *this;
}

PolicyBuilder::PolicyBuilder(PolicyBuilder&& other)
    : m_pImpl(std::move(*other.m_pImpl)) { }

PolicyBuilder& PolicyBuilder::operator=(PolicyBuilder&& other) {
    if (this != &other) {
        std::swap(*m_pImpl, *other.m_pImpl);
    }
    return *this;
}

PolicyBuilder::~PolicyBuilder() = default;

std::shared_ptr<const core::Boolean> PolicyBuilder::add_boolean_feature(core::Boolean boolean) {
    return m_pImpl->add_boolean_feature(boolean);
}

std::shared_ptr<const core::Numerical> PolicyBuilder::add_numerical_feature(core::Numerical numerical) {
    return m_pImpl->add_numerical_feature(numerical);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_pos_condition(std::shared_ptr<const core::Boolean> b) {
    return m_pImpl->add_pos_condition(b);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_neg_condition(std::shared_ptr<const core::Boolean> b) {
    return m_pImpl->add_neg_condition(b);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_gt_condition(std::shared_ptr<const core::Numerical> n) {
    return m_pImpl->add_gt_condition(n);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_eq_condition(std::shared_ptr<const core::Numerical> n) {
    return m_pImpl->add_eq_condition(n);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_pos_effect(std::shared_ptr<const core::Boolean> b) {
    return m_pImpl->add_pos_effect(b);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_neg_effect(std::shared_ptr<const core::Boolean> b) {
    return m_pImpl->add_neg_effect(b);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_bot_effect(std::shared_ptr<const core::Boolean> b) {
    return m_pImpl->add_bot_effect(b);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_inc_effect(std::shared_ptr<const core::Numerical> n) {
    return m_pImpl->add_inc_effect(n);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_dec_effect(std::shared_ptr<const core::Numerical> n) {
    return m_pImpl->add_dec_effect(n);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_bot_effect(std::shared_ptr<const core::Numerical> n) {
    return m_pImpl->add_bot_effect(n);
}

std::shared_ptr<const Rule> PolicyBuilder::add_rule(
    std::vector<std::shared_ptr<const BaseCondition>>&& conditions,
    std::vector<std::shared_ptr<const BaseEffect>>&& effects) {
    return m_pImpl->add_rule(std::move(conditions), std::move(effects));
}

Policy PolicyBuilder::get_result() {
    return m_pImpl->get_result();
}


PolicyMinimizer::PolicyMinimizer() { }

PolicyMinimizer::PolicyMinimizer(const PolicyMinimizer& other) = default;

PolicyMinimizer& PolicyMinimizer::operator=(const PolicyMinimizer& other) = default;

PolicyMinimizer::PolicyMinimizer(PolicyMinimizer&& other) = default;

PolicyMinimizer& PolicyMinimizer::operator=(PolicyMinimizer&& other) = default;

PolicyMinimizer::~PolicyMinimizer() { }

Policy PolicyMinimizer::minimize_greedy(const Policy& policy, const core::StatePairs& true_state_pairs, const core::StatePairs& false_state_pairs) const {
    /*
       Idea: A rule C -> E dominates a rules C' -> E' if C subseteq C', E subseteq E'
             and substituting C' -> E' with C -> E in policy leads to consistent classification.
    */
    /* 1. try to remove conditions such that classification remains.
    */
    /* 2. try to remove effects such that classification remains.
    */
    throw std::runtime_error("Not implemented.");
}


PolicyReader::PolicyReader() = default;

PolicyReader::PolicyReader(const PolicyReader& other)
    : m_pImpl(*other.m_pImpl) { }

PolicyReader& PolicyReader::operator=(const PolicyReader& other) {
    if (this != &other) {
        *m_pImpl = *other.m_pImpl;
    }
    return *this;
}

PolicyReader::PolicyReader(PolicyReader&& other)
    : m_pImpl(std::move(*other.m_pImpl)) { }

PolicyReader& PolicyReader::operator=(PolicyReader&& other) {
    if (this != &other) {
        std::swap(*m_pImpl, *other.m_pImpl);
    }
    return *this;
}

PolicyReader::~PolicyReader() = default;

Policy PolicyReader::read(const std::string& data, core::SyntacticElementFactory& factory) const {
    return m_pImpl->read(data, factory);
}


PolicyWriter::PolicyWriter() { }

PolicyWriter::PolicyWriter(const PolicyWriter& other)
    : m_pImpl(*other.m_pImpl) { }

PolicyWriter& PolicyWriter::operator=(const PolicyWriter& other) {
    if (this != &other) {
        *m_pImpl = *other.m_pImpl;
    }
    return *this;
}

PolicyWriter::PolicyWriter(PolicyWriter&& other)
    : m_pImpl(std::move(*other.m_pImpl)) { }

PolicyWriter& PolicyWriter::operator=(PolicyWriter&& other) {
    if (this != &other) {
        std::swap(*m_pImpl, *other.m_pImpl);
    }
    return *this;
}

PolicyWriter::~PolicyWriter() { }

std::string PolicyWriter::write(const Policy& policy) const {
    return m_pImpl->write(policy);
}

}
