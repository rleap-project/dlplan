#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_

#include "../utils.h"
#include "../../../../include/dlplan/core.h"

#include <sstream>
#include <memory>

using namespace std::string_literals;


namespace dlplan::utils {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class TopRole : public Role {
private:
    RoleDenotation evaluate_impl(const State& state, DenotationsCaches&) const override {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        denotation.set();
        return denotation;
    }

    RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        RoleDenotations denotations;
        denotations.reserve(states.size());
        for (size_t i = 0; i < states.size(); ++i) {
            RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
            denotation.set();
            denotations.push_back(caches.role_denotation_cache.insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

    TopRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info)
        : Role(vocabulary_info, index, true) { }

    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Role& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const TopRole&>(other);
            return m_is_static == other_derived.m_is_static;
        }
        return false;
    }

    size_t hash() const {
        return dlplan::utils::hash_combine(m_is_static);
    }

    RoleDenotation evaluate(const State& state) const override {
        auto denotation = RoleDenotation(state.get_instance_info()->get_objects().size());
        denotation.set();
        return denotation;

    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "r_top";
    }

    int compute_evaluate_time_score() const override {
        return SCORE_CONSTANT;
    }
};

}


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::TopRole>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::TopRole>& left_role,
            const std::shared_ptr<const dlplan::core::TopRole>& right_role) const {
            return *left_role < *right_role;
        }
    };

    template<>
    struct hash<dlplan::core::TopRole>
    {
        std::size_t operator()(const dlplan::core::TopRole& role) const {
            return role.hash();
        }
    };
}

#endif
