#include "../../include/dlplan/policy.h"

#include "../../include/dlplan/core.h"
#include "../../include/dlplan/utils/hash.h"
#include "../../include/dlplan/policy/condition.h"
#include "../../include/dlplan/policy/effect.h"

#include <algorithm>
#include <sstream>


namespace dlplan::policy {

struct InsertNamedElementFromCondition : public BaseConditionVisitor {
    Booleans& booleans;
    Numericals& numericals;
    Concepts& concepts;

    InsertNamedElementFromCondition(Booleans& booleans_, Numericals& numericals_, Concepts& concepts_)
        : booleans(booleans_), numericals(numericals_), concepts(concepts_) { }

    void visit(const std::shared_ptr<const PositiveBooleanCondition>& condition) override {
        booleans.insert(condition->get_named_element());
    }

    void visit(const std::shared_ptr<const NegativeBooleanCondition>& condition) override {
        booleans.insert(condition->get_named_element());
    }

    void visit(const std::shared_ptr<const GreaterNumericalCondition>& condition) override {
        numericals.insert(condition->get_named_element());
    }

    void visit(const std::shared_ptr<const EqualNumericalCondition>& condition) override {
        numericals.insert(condition->get_named_element());
    }

    void visit(const std::shared_ptr<const GreaterConceptCondition>& condition) override {
        concepts.insert(condition->get_named_element());
    }

    void visit(const std::shared_ptr<const EqualConceptCondition>& condition) override {
        concepts.insert(condition->get_named_element());
    }
};


struct InsertNamedElementFromEffect : public BaseEffectVisitor {
    Booleans& booleans;
    Numericals& numericals;
    Concepts& concepts;

    InsertNamedElementFromEffect(Booleans& booleans_, Numericals& numericals_, Concepts& concepts_)
        : booleans(booleans_), numericals(numericals_), concepts(concepts_) { }

    void visit(const std::shared_ptr<const PositiveBooleanEffect>& effect) override {
        booleans.insert(effect->get_named_element());
    }

    void visit(const std::shared_ptr<const NegativeBooleanEffect>& effect) override {
        booleans.insert(effect->get_named_element());
    }

    void visit(const std::shared_ptr<const UnchangedBooleanEffect>& effect) override {
        booleans.insert(effect->get_named_element());
    }

    void visit(const std::shared_ptr<const IncrementNumericalEffect>& effect) override {
        numericals.insert(effect->get_named_element());
    }

    void visit(const std::shared_ptr<const IncrementOrUnchangedNumericalEffect>& effect) override {
        numericals.insert(effect->get_named_element());
    }

    void visit(const std::shared_ptr<const DecrementNumericalEffect>& effect) override {
        numericals.insert(effect->get_named_element());
    }

    void visit(const std::shared_ptr<const DecrementOrUnchangedNumericalEffect>& effect) override {
        numericals.insert(effect->get_named_element());
    }

    void visit(const std::shared_ptr<const UnchangedNumericalEffect>& effect) override {
        numericals.insert(effect->get_named_element());
    }

    void visit(const std::shared_ptr<const GreaterNumericalEffect>& effect) override {
        numericals.insert(effect->get_named_element());
    }

    void visit(const std::shared_ptr<const EqualNumericalEffect>& effect) override {
        numericals.insert(effect->get_named_element());
    }

    void visit(const std::shared_ptr<const IncrementConceptEffect>& effect) override {
        concepts.insert(effect->get_named_element());
    }

    void visit(const std::shared_ptr<const DecrementConceptEffect>& effect) override {
        concepts.insert(effect->get_named_element());
    }

    void visit(const std::shared_ptr<const UnchangedConceptEffect>& effect) override {
        concepts.insert(effect->get_named_element());
    }

    void visit(const std::shared_ptr<const GreaterConceptEffect>& effect) override {
        concepts.insert(effect->get_named_element());
    }

    void visit(const std::shared_ptr<const EqualConceptEffect>& effect) override {
        concepts.insert(effect->get_named_element());
    }
};


Policy::Policy(int identifier, const Rules& rules)
    : Base<Policy>(identifier), m_rules(rules) {
    // Retrieve boolean, numericals, and concepts from the rules.
    InsertNamedElementFromCondition condition_visitor(m_booleans, m_numericals, m_concepts);
    InsertNamedElementFromEffect effect_visitor(m_booleans, m_numericals, m_concepts);
    for (const auto& rule : m_rules) {
        for (const auto& condition : rule->get_conditions()) {
            condition->accept(condition_visitor);
        }
        for (const auto& effect : rule->get_effects()) {
            effect->accept(effect_visitor);
        }
    }
}

Policy::Policy(const Policy& other) = default;

Policy& Policy::operator=(const Policy& other) = default;

Policy::Policy(Policy&& other) = default;

Policy& Policy::operator=(Policy&& other) = default;

Policy::~Policy() = default;

bool Policy::are_equal_impl(const Policy& other) const {
    if (this != &other) {
        return m_booleans == other.m_booleans
            && m_numericals == other.m_numericals
            && m_rules == other.m_rules;
    }
    return true;
}

size_t Policy::hash_impl() const {
    return hash_combine(
        hash_set(m_booleans),
        hash_set(m_numericals),
        hash_set(m_rules));
}

std::shared_ptr<const Rule> Policy::evaluate(const core::State& source_state, const core::State& target_state) const {
    for (const auto& r : m_rules) {
        if (r->evaluate_conditions(source_state) && r->evaluate_effects(source_state, target_state)) {
            return r;
        }
    }
    return nullptr;
}

std::shared_ptr<const Rule> Policy::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    for (const auto& r : m_rules) {
        if (r->evaluate_conditions(source_state, caches) && r->evaluate_effects(source_state, target_state, caches)) {
            return r;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<const Rule>> Policy::evaluate_conditions(const core::State& source_state) const {
    std::vector<std::shared_ptr<const Rule>> result;
    for (const auto& r : m_rules) {
        if (r->evaluate_conditions(source_state)) {
            result.push_back(r);
        }
    }
    return result;
}

std::vector<std::shared_ptr<const Rule>> Policy::evaluate_conditions(const core::State& source_state, core::DenotationsCaches& caches) const {
    std::vector<std::shared_ptr<const Rule>> result;
    for (const auto& r : m_rules) {
        if (r->evaluate_conditions(source_state, caches)) {
            result.push_back(r);
        }
    }
    return result;
}

std::shared_ptr<const Rule> Policy::evaluate_effects(const core::State& source_state, const core::State& target_state, const std::vector<std::shared_ptr<const Rule>>& rules) const {
    for (const auto& r : rules) {
        if (r->evaluate_effects(source_state, target_state)) {
            return r;
        }
    }
    return nullptr;
}

std::shared_ptr<const Rule> Policy::evaluate_effects(const core::State& source_state, const core::State& target_state, const std::vector<std::shared_ptr<const Rule>>& rules, core::DenotationsCaches& caches) const {
    for (const auto& r : rules) {
        if (r->evaluate_effects(source_state, target_state, caches)) {
            return r;
        }
    }
    return nullptr;
}

void Policy::str_impl(std::stringstream& out) const {
    out << "(:policy\n";
    out << "(:booleans ";
    for (const auto& boolean : m_booleans) {
        out << "(" << boolean->get_key() << " \"" << boolean->get_element()->str() << "\")";
        if (boolean != *m_booleans.rbegin()) out << " ";
    }
    out << ")\n";
    out << "(:numericals ";
    for (const auto& numerical : m_numericals) {
        out << "(" << numerical->get_key() << " \"" << numerical->get_element()->str() << "\")";
        if (numerical != *m_numericals.rbegin()) out << " ";
    }
    out << ")\n";
    for (const auto& rule : m_rules) {
        out << rule->str() << "\n";
    }
    out << ")";
}

int Policy::compute_evaluate_time_score() const {
    int score = 0;
    for (const auto& rule : m_rules) {
        score += rule->compute_evaluate_time_score();
    }
    return score;
}

const Booleans& Policy::get_booleans() const {
    return m_booleans;
}

const Numericals& Policy::get_numericals() const {
    return m_numericals;
}

const Concepts& Policy::get_concepts() const {
    return m_concepts;
}

const Rules& Policy::get_rules() const {
    return m_rules;
}

}
