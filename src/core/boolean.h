#ifndef DLPLAN_SRC_CORE_BOOLEAN_H_
#define DLPLAN_SRC_CORE_BOOLEAN_H_

#include <memory>

#include "elements/boolean.h"

#include "../../include/dlplan/core.h"


namespace dlplan::core {

class BooleanImpl {
public:
    BooleanImpl(std::shared_ptr<const element::Boolean>& element) {
        if (!element) {
            throw std::runtime_error("BooleanImpl::BooleanImpl - tried to construct Boolean from nullptr");
        }
    }

    bool evaluate(const Boolean* parent, const State& state, EvaluationCaches& caches) const {
        if (state.get_instance_info()->get_vocabulary_info() != parent->get_vocabulary_info()) {
            throw std::runtime_error("BooleanImpl::evaluate - mismatched vocabularies of Boolean and State.");
        }
        if (state.get_instance_info() != caches.get_instance_info()) {
            throw std::runtime_error("BooleanImpl::evaluate - mismatches instances of State and EvaluationCaches.");
        }
        return parent->get_element()->evaluate(state, caches);
    }
};

}

#endif
