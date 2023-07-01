#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_IDENTITY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_IDENTITY_H_

#include "../utils.h"

#include "../../../../include/dlplan/core.h"

#include <sstream>

using namespace std::string_literals;


namespace dlplan::core {

class IdentityRole : public Role {
private:
    void compute_result(const ConceptDenotation& denot, RoleDenotation& result) const {
        for (const auto& single : denot.to_vector()) {
            result.insert(std::make_pair(single, single));
        }
    }

    RoleDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            *m_concept->evaluate(state, caches),
            denotation);
        return denotation;
    }

    RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        RoleDenotations denotations;
        denotations.reserve(states.size());
        auto concept_denotations = m_concept->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                *(*concept_denotations)[i],
                denotation);
            denotations.push_back(caches.get_role_denotation_cache().insert_denotation(std::move(denotation)));
        }
       return denotations;
    }

protected:
    const std::shared_ptr<const Concept> m_concept;

public:
    IdentityRole(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const Concept> concept)
    : Role(vocabulary_info, concept->is_static()), m_concept(concept) {
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

    int compute_evaluate_time_score() const override {
        return m_concept->compute_evaluate_time_score() + SCORE_LINEAR;
    }

    static std::string get_name() {
        return "r_identity";
    }
};

}

#endif
