#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_IDENTITY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_IDENTITY_H_

#include "../role.h"
#include "../concept.h"


namespace dlplan::core::element {

class IdentityRole : public Role {
private:
    void compute_result(const ConceptDenotation& denot, RoleDenotation& result) const {
        for (const auto& single : denot) {
            result.insert(std::make_pair(single, single));
        }
    }

protected:
    const Concept_Ptr m_concept;

public:
    IdentityRole(const VocabularyInfo& vocabulary, Concept_Ptr concept)
    : Role(vocabulary), m_concept(concept) {
        if (!concept) {
            throw std::runtime_error("IdentityRole::IdentityRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        RoleDenotation denotation(state.get_instance_info_ref().get_num_objects());
        compute_result(
            m_concept->evaluate(state),
            denotation);
        return denotation;
    }

    DENOTS<RoleDenotation*>* evaluate(const States& states, DenotationsCaches& caches) const override {
        auto cached = caches.m_r_denots_cache.find(get_index());
        if (cached) return cached;
        auto denotations = caches.m_r_denots_cache.get_new_denotations();
        auto concept_denotations = m_concept->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            const auto& state = states[i];
            int num_objects = state.get_instance_info_ref().get_num_objects();
            auto denotation = caches.m_r_denot_cache.get_new_denotation(num_objects);
            compute_result(
                *(*concept_denotations)[i],
                *denotation);
            denotations->push_back(caches.m_r_denot_cache.insert(std::move(denotation)));
        }
        return caches.m_r_denots_cache.insert(std::move(denotations), get_index());
    }

    int compute_complexity() const override {
        return m_concept->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_concept->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "r_identity";
    }
};

}

#endif
