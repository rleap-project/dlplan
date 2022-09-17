#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_IDENTITY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_IDENTITY_H_

#include "../role.h"
#include "../concept.h"


namespace dlplan::core::element {

class IdentityRole : public Role {
private:
    RoleDenotation compute_result(ConceptDenotation&& denot, RoleDenotation&& result) const {
        for (const auto& single : denot) {
            result.insert(std::make_pair(single, single));
        }
        return result;
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
        auto bot_role = RoleDenotation(state.get_instance_info()->get_num_objects());
        return compute_result(
            m_concept->evaluate(state),
            std::move(bot_role));
    }

    RoleDenotation evaluate(const State& state, EvaluationCaches& cache) const override {
        if (cache.m_role_denotation_cache.count(state, *this)) {
            return cache.m_role_denotation_cache.find(state, *this);
        }
        auto bot_role = RoleDenotation(state.get_instance_info()->get_num_objects());
        auto result = compute_result(
            m_concept->evaluate(state, cache),
            std::move(bot_role));
        cache.m_role_denotation_cache.insert(state, *this, result);
        return result;
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
