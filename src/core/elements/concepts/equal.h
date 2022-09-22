#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_EQUAL_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_EQUAL_H_

#include "../concept.h"
#include "../role.h"


namespace dlplan::core::element {

class EqualConcept : public Concept {
private:
    void compute_result(const RoleDenotation& left_denot, const RoleDenotation& right_denot, ConceptDenotation& result) const {
        // find counterexample [(a,b) in R and (a,b) not in S] or [(a,b) not in R and (a,b) in S]
        for (const auto& pair : left_denot) {
            if (!right_denot.contains(pair)) result.erase(pair.first);
        }
        for (const auto& pair : right_denot) {
            if (!left_denot.contains(pair)) result.erase(pair.first);
        }
    }

protected:
    const Role_Ptr m_role_left;
    const Role_Ptr m_role_right;

public:
    EqualConcept(const VocabularyInfo& vocabulary, Role_Ptr role_left, Role_Ptr role_right)
    : Concept(vocabulary), m_role_left(role_left), m_role_right(role_right) {
        if (!(role_left && role_right)) {
            throw std::runtime_error("EqualConcept::EqualConcept - at least one child is a nullptr");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        auto denotation = state.get_instance_info_ref().get_top_concept_ref();
        compute_result(
            m_role_left->evaluate(state),
            m_role_right->evaluate(state),
            denotation);
        return denotation;
    }

    DENOTS<ConceptDenotation*>* evaluate(const States& states, DenotationsCaches& caches) const override {
        // check if denotations is cached.
        auto cached = caches.m_c_denots_mapping.find(get_index());
        if (cached) return cached;
        // allocate memory for new denotations
        auto denotations = caches.m_c_denots_cache.get_new_entry();
        denotations->reserve(states.size());
        // get denotations of children
        auto role_left_denotations = m_role_left->evaluate(states, caches);
        auto role_right_denotations = m_role_right->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            const auto& state = states[i];
            int num_objects = state.get_instance_info_ref().get_num_objects();
            auto denotation = caches.m_c_denot_cache.get_new_entry(num_objects);
            denotation->set();
            compute_result(
                *(*role_left_denotations)[i],
                *(*role_right_denotations)[i],
                *denotation);
            // register denotation and append it to denotations.
            denotations->push_back(caches.m_c_denot_cache.insert(std::move(denotation)).first->get());
        }
        // register denotations and return it.
        auto result_denotations = caches.m_c_denots_cache.insert(std::move(denotations)).first->get();
        caches.m_c_denots_mapping.insert(result_denotations, get_index());
        return result_denotations;
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
        return "c_equal";
    }
};

}

#endif
