#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_EQUAL_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_EQUAL_H_

#include "../utils.h"

#include "../../../../include/dlplan/core.h"

#include <sstream>

using namespace std::string_literals;


namespace dlplan::core {

class EqualConcept : public Concept {
private:
    void compute_result(const RoleDenotation& left_denot, const RoleDenotation& right_denot, ConceptDenotation& result) const {
        // find counterexample [(a,b) in R and (a,b) not in S] or [(a,b) not in R and (a,b) in S]
        result.set();
        for (const auto& pair : left_denot.to_vector()) {
            if (!right_denot.contains(pair)) result.erase(pair.first);
        }
        for (const auto& pair : right_denot.to_vector()) {
            if (!left_denot.contains(pair)) result.erase(pair.first);
        }
    }

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            *m_role_left->evaluate(state, caches),
            *m_role_right->evaluate(state, caches),
            denotation);
        return denotation;
    }

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        ConceptDenotations denotations;
        denotations.reserve(states.size());
        auto role_left_denotations = m_role_left->evaluate(states, caches);
        auto role_right_denotations = m_role_right->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                *(*role_left_denotations)[i],
                *(*role_right_denotations)[i],
                denotation);
            // register denotation and append it to denotations.
            denotations.push_back(caches.get_concept_denotation_cache().insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

protected:
    const std::shared_ptr<const Role> m_role_left;
    const std::shared_ptr<const Role> m_role_right;

public:
    EqualConcept(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role_left, std::shared_ptr<const Role> role_right)
    : Concept(vocabulary_info, role_left->is_static() && role_right->is_static()),
      m_role_left(role_left), m_role_right(role_right) {
        if (!(role_left && role_right)) {
            throw std::runtime_error("EqualConcept::EqualConcept - at least one child is a nullptr");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        auto denotation = ConceptDenotation(state.get_instance_info()->get_objects().size());
        compute_result(
            m_role_left->evaluate(state),
            m_role_right->evaluate(state),
            denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return m_role_left->compute_complexity() + m_role_right->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_role_left->compute_repr(out);
        out << ",";
        m_role_right->compute_repr(out);
        out << ")";
    }

    int compute_evaluate_time_score() const override {
        return m_role_left->compute_evaluate_time_score() + m_role_right->compute_evaluate_time_score() + SCORE_QUADRATIC;
    }

    static std::string get_name() {
        return "c_equal";
    }
};

}

#endif
