#ifndef DLPLAN_SRC_POLICY_EVALUATION_CACHE_H_
#define DLPLAN_SRC_POLICY_EVALUATION_CACHE_H_

#include "../../include/dlplan/evaluator.h"

#include "../../include/dlplan/core.h"


namespace dlplan::policy {

class EvaluationCache {
private:
    evaluator::BooleanEvaluator m_boolean_evaluator;
    evaluator::NumericalEvaluator m_numerical_evaluator;

public:
    EvaluationCache(
        int num_boolean_features, int num_numerical_features);

    evaluator::BooleanEvaluator& get_boolean_evaluator();

    const evaluator::BooleanEvaluator& get_boolean_evaluator() const;

    evaluator::NumericalEvaluator& get_numerical_cache();

    const evaluator::NumericalEvaluator& get_numerical_cache() const;
};

}

#endif
