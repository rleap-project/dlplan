#ifndef DLPLAN_SRC_CORE_ROLE_H_
#define DLPLAN_SRC_CORE_ROLE_H_


namespace dlplan::core {

class RoleImpl {
public:
    RoleImpl(std::shared_ptr<const element::Role>& element) {
        if (!element) {
            throw std::runtime_error("RoleImpl::RoleImpl - tried to construct Role from nullptr");
        }
    }

    RoleDenotation evaluate(const Role* parent, const State& state) const {
        if (state.get_instance_info()->get_vocabulary_info() != parent->get_vocabulary_info()) {
            throw std::runtime_error("RoleImpl::evaluate - mismatched vocabularies of Role and State.");
        }
        return parent->get_element()->evaluate(state);
    }
};

}

#endif
