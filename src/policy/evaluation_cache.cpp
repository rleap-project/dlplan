#include "evaluation_cache.h"


namespace dlplan::policy {

EvaluationCache::EvaluationCache(int num_boolean_features, int num_numerical_features) :
    m_boolean_evaluator(num_boolean_features),
    m_numerical_evaluator(num_numerical_features) { }


evaluator::BooleanEvaluator& EvaluationCache::get_boolean_evaluator() {
    return m_boolean_evaluator;
}

const evaluator::BooleanEvaluator& EvaluationCache::get_boolean_evaluator() const {
    return m_boolean_evaluator;
}

evaluator::NumericalEvaluator& EvaluationCache::get_numerical_cache() {
    return m_numerical_evaluator;
}

const evaluator::NumericalEvaluator& EvaluationCache::get_numerical_cache() const {
    return m_numerical_evaluator;
}

}
