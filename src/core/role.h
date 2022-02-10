#ifndef DLPLAN_SRC_CORE_ROLE_H_
#define DLPLAN_SRC_CORE_ROLE_H_

#include <memory>

#include "elements/role.h"

#include "../../include/dlplan/core.h"


namespace dlplan::core {

class RoleImpl {
public:
    RoleImpl(std::shared_ptr<const element::Role>& element) {
        if (!element) {
            throw std::runtime_error("RoleImpl::RoleImpl - tried to construct Role from nullptr");
        }
    }

    RoleDenotation evaluate(const Role* parent, const State& state, EvaluationCaches& caches) const {
        if (state.get_instance_info()->get_vocabulary_info() != parent->get_vocabulary_info()) {
            throw std::runtime_error("RoleImpl::evaluate - mismatched vocabularies of Role and State.");
        }
        if (state.get_instance_info() != caches.get_instance_info()) {
            throw std::runtime_error("RoleImpl::evaluate - mismatches instances of State and EvaluationCaches.");
        }
        return parent->get_element()->evaluate(state, caches);
    }
};

}

#endif
