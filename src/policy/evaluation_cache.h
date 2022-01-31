#ifndef DLPLAN_SRC_POLICY_EVALUATION_CACHE_H_
#define DLPLAN_SRC_POLICY_EVALUATION_CACHE_H_

#include "../../include/dlplan/evaluator.h"


namespace dlplan::policy {

class EvaluationCaches {
private:
    evaluator::BooleanEvaluator m_boolean_evaluator;
    evaluator::NumericalEvaluator m_numerical_evaluator;

public:
    // HACK: we set minimum size to 1 because of division by zero
    EvaluationCaches(int num_boolean_features, int num_numerical_features) :
      m_boolean_evaluator(num_boolean_features),
      m_numerical_evaluator(num_numerical_features) { }


    evaluator::BooleanEvaluator& get_boolean_evaluator() {
        return m_boolean_evaluator;
    }

    const evaluator::BooleanEvaluator& get_boolean_evaluator() const {
        return m_boolean_evaluator;
    }

    evaluator::NumericalEvaluator& get_numerical_cache() {
        return m_numerical_evaluator;
    }

    const evaluator::NumericalEvaluator& get_numerical_cache() const {
        return m_numerical_evaluator;
    }
};

}

#endif
