#include "../../include/dlplan/evaluator.h"

#include "boolean_evaluator.h"
#include "numerical_evaluator.h"


namespace dlplan::evaluator {

BooleanEvaluator::BooleanEvaluator(int num_booleans) : m_pImpl(num_booleans) { }

BooleanEvaluator::BooleanEvaluator(const BooleanEvaluator& other)
    : m_pImpl(BooleanEvaluatorImpl(*other.m_pImpl)) { }

BooleanEvaluator& BooleanEvaluator::operator=(const BooleanEvaluator& other) {
    if (this != &other) {
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

BooleanEvaluator::~BooleanEvaluator() { }

bool BooleanEvaluator::evaluate(int boolean_index, const core::Boolean& boolean, int state_index, const core::State& state, core::PerElementEvaluationCache& caches) {
    return m_pImpl->evaluate(boolean_index, boolean, state_index, state, caches);
}


NumericalEvaluator::NumericalEvaluator(int num_numericals) : m_pImpl(num_numericals) { }

NumericalEvaluator::NumericalEvaluator(const NumericalEvaluator& other)
    : m_pImpl(NumericalEvaluatorImpl(*other.m_pImpl)) { }

NumericalEvaluator& NumericalEvaluator::operator=(const NumericalEvaluator& other) {
    if (this != &other) {
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

NumericalEvaluator::~NumericalEvaluator() { }

int NumericalEvaluator::evaluate(int numerical_index, const core::Numerical& numerical, int state_index, const core::State& state, core::PerElementEvaluationCache& caches) {
    return m_pImpl->evaluate(numerical_index, numerical, state_index, state, caches);
}

}