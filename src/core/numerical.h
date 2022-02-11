#ifndef DLPLAN_SRC_CORE_NUMERICAL_H_
#define DLPLAN_SRC_CORE_NUMERICAL_H_

#include <memory>

#include "elements/numerical.h"

#include "../../include/dlplan/core.h"


namespace dlplan::core {

class NumericalImpl {
public:
    NumericalImpl(std::shared_ptr<const element::Numerical>& element)  {
        if (!element) {
            throw std::runtime_error("NumericalImpl::NumericalImpl - tried to construct Numerical from nullptr");
        }
    }

    int evaluate(const Numerical* parent, const State& state, PerElementEvaluationCache& caches) const {
        if (state.get_instance_info()->get_vocabulary_info() != parent->get_vocabulary_info()) {
            throw std::runtime_error("NumericalImpl::evaluate - mismatched vocabularies of Numerical and State.");
        }
        if (state.get_instance_info() != caches.get_instance_info()) {
            throw std::runtime_error("NumericalImpl::evaluate - mismatches instances of State and EvaluationCaches.");
        }
        return parent->get_element()->evaluate(state, caches);
    }
};

}

#endif
