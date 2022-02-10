#include "../../include/dlplan/evaluator.h"

#include "boolean_evaluator.h"
#include "numerical_evaluator.h"

namespace dlplan::evaluator {

BooleanEvaluator::BooleanEvaluator(int num_booleans) : m_pImpl(num_booleans) { }

BooleanEvaluator::~BooleanEvaluator() { }

bool BooleanEvaluator::evaluate(int boolean_index, const core::Boolean& boolean, int state_index, const core::State& state, core::EvaluationCaches& caches) {
    return m_pImpl->evaluate(boolean_index, boolean, state_index, state, caches);
}


NumericalEvaluator::NumericalEvaluator(int num_numericals) : m_pImpl(num_numericals) { }

NumericalEvaluator::~NumericalEvaluator() { }

int NumericalEvaluator::evaluate(int numerical_index, const core::Numerical& numerical, int state_index, const core::State& state, core::EvaluationCaches& caches) {
    return m_pImpl->evaluate(numerical_index, numerical, state_index, state, caches);
}

}