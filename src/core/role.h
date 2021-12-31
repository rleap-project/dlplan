#ifndef DLPLAN_SRC_CORE_ROLE_H_
#define DLPLAN_SRC_CORE_ROLE_H_

#include <algorithm>

#include "element.h"


namespace dlplan::core {

class RoleImpl : public ElementImpl<RoleDenotation> {
public:
    RoleImpl(const std::shared_ptr<element::Element<RoleDenotation>>& element) {
        if (!element) {
            throw std::runtime_error("RoleImpl::RoleImpl - tried to construct Role from nullptr");
        }
    }
    ~RoleImpl() override = default;

    RoleDenotation evaluate(const Element<RoleDenotation>* parent, const State& state) const override {
        if (state.get_instance_info()->get_vocabulary_info() != parent->get_vocabulary_info()) {
            throw std::runtime_error("RoleImpl::evaluate - mismatched vocabularies of Role and State.");
        }
        return parent->get_element()->evaluate(state);
    }
};

}

#endif
