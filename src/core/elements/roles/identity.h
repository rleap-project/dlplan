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

    std::unique_ptr<RoleDenotation> evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        auto denotation = std::make_unique<RoleDenotation>(
            RoleDenotation(state.get_instance_info()->get_objects().size()));
        compute_result(
            *m_concept->evaluate(state, caches),
            *denotation);
        return denotation;
    }

    std::unique_ptr<RoleDenotations> evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        auto denotations = std::make_unique<RoleDenotations>();
        denotations->reserve(states.size());
        auto concept_denotations = m_concept->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            auto denotation = std::make_unique<RoleDenotation>(
                RoleDenotation(states[i].get_instance_info()->get_objects().size()));
            compute_result(
                *(*concept_denotations)[i],
                *denotation);
            denotations->push_back(caches.m_r_denot_cache.insert(std::move(denotation)).first->get());
        }
       return denotations;
    }

protected:
    const Concept_Ptr m_concept;

public:
    IdentityRole(const VocabularyInfo& vocabulary, Concept_Ptr concept)
    : Role(vocabulary, concept->is_static()), m_concept(concept) {
        if (!concept) {
            throw std::runtime_error("IdentityRole::IdentityRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            m_concept->evaluate(state),
            denotation);
        return denotation;
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
