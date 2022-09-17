#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_SUBSET_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_SUBSET_H_

#include "../concept.h"
#include "../role.h"


namespace dlplan::core::element {

class SubsetConcept : public Concept {
private:
    ConceptDenotation compute_result(RoleDenotation&& left_denot, RoleDenotation&& right_denot, ConceptDenotation&& result) const {
        // find counterexamples a : exists b . (a,b) in R and (a,b) notin S
        for (const auto& pair : left_denot) {
            if (!right_denot.contains(pair)) result.erase(pair.first);
        }
        return result;
    }

protected:
    const Role_Ptr m_role_left;
    const Role_Ptr m_role_right;

public:
    SubsetConcept(const VocabularyInfo& vocabulary, Role_Ptr role_left, Role_Ptr role_right)
    : Concept(vocabulary), m_role_left(role_left), m_role_right(role_right) {
        if (!(role_left && role_right)) {
            throw std::runtime_error("SubsetConcept::SubsetConcept - at least one child is a nullptr");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        auto top_concept = state.get_instance_info()->get_top_concept();
        return compute_result(
            m_role_left->evaluate(state),
            m_role_right->evaluate(state),
            std::move(top_concept));
    }

    ConceptDenotation evaluate(const State& state, EvaluationCaches& cache) const override {
        if (cache.m_concept_denotation_cache.count(state, *this)) {
            return cache.m_concept_denotation_cache.find(state, *this);
        }
        auto top_concept = state.get_instance_info()->get_top_concept();
        auto result = compute_result(
            m_role_left->evaluate(state, cache),
            m_role_right->evaluate(state, cache),
            std::move(top_concept));
        cache.m_concept_denotation_cache.insert(state, *this, result);
        return result;
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

    static std::string get_name() {
        return "c_subset";
    }
};

}

#endif
