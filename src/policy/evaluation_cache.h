#ifndef DLPLAN_SRC_POLICY_EVALUATION_CACHE_H_
#define DLPLAN_SRC_POLICY_EVALUATION_CACHE_H_

#include "../../include/dlplan/evaluator.h"

#include "../../include/dlplan/core.h"


namespace dlplan {
namespace core {
    class InstanceInfo;
}
namespace policy {

class EvaluationCache {
private:
    evaluator::BooleanEvaluator m_boolean_evaluator;
    evaluator::NumericalEvaluator m_numerical_evaluator;

    core::PerElementEvaluationCache m_element_cache;

public:
    EvaluationCache(
        std::shared_ptr<const core::InstanceInfo> instance_info,
        int num_boolean_features, int num_numerical_features);

    evaluator::BooleanEvaluator& get_boolean_evaluator();

    const evaluator::BooleanEvaluator& get_boolean_evaluator() const;

    evaluator::NumericalEvaluator& get_numerical_cache();

    const evaluator::NumericalEvaluator& get_numerical_cache() const;

    core::PerElementEvaluationCache& get_element_cache();

    const core::PerElementEvaluationCache& get_element_cache() const;
};

}
}

#endif
