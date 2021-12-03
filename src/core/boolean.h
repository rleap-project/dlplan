#ifndef DLPLAN_SRC_CORE_BOOLEAN_H_
#define DLPLAN_SRC_CORE_BOOLEAN_H_

#include "element.h"


namespace dlplan::core {

class BooleanImpl : public ElementImpl<bool> {
public:
    BooleanImpl(const std::shared_ptr<element::Element<bool>>& element) {
        if (!element) {
            throw std::runtime_error("BooleanImpl::BooleanImpl - tried to construct Boolean from nullptr");
        }
    }
    ~BooleanImpl() override = default;

    bool evaluate(const Element<bool>* parent, const State& state) const override {
        if (state.get_instance_info()->get_vocabulary_info() != parent->get_vocabulary_info()) {
            throw std::runtime_error("BooleanImpl::evaluate - mismatched vocabularies of Boolean and State.");
        }
        return parent->get_element()->evaluate(state);
    }
};

}

#endif
