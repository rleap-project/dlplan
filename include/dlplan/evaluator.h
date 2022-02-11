#ifndef DLPLAN_INCLUDE_DLPLAN_EVALUATOR_H_
#define DLPLAN_INCLUDE_DLPLAN_EVALUATOR_H_

#include <vector>

#include "pimpl.h"


namespace dlplan {
namespace core {
    class Boolean;
    class Numerical;
    class State;
    class PerElementEvaluationCache;
}
namespace evaluator {
class BooleanEvaluatorImpl;
class NumericalEvaluatorImpl;

/**
 * An Evaluator caches feature evaluations to speed up subsequent lookups.
 *
 * Indices for Element and State are needed because evaluations
 * are stored in a continuous memory block for improved cache locality.
 */
class BooleanEvaluator {
private:
    pimpl<BooleanEvaluatorImpl> m_pImpl;

public:
    BooleanEvaluator(int num_booleans);
    BooleanEvaluator(const BooleanEvaluator& other);
    BooleanEvaluator& operator=(const BooleanEvaluator& other);
    ~BooleanEvaluator();

    bool evaluate(int boolean_index, const core::Boolean& boolean, int state_index, const core::State& state, core::PerElementEvaluationCache& caches);

    void clear();
};


class NumericalEvaluator {
private:
    pimpl<NumericalEvaluatorImpl> m_pImpl;

public:
    NumericalEvaluator(int num_numericals);
    NumericalEvaluator(const NumericalEvaluator& other);
    NumericalEvaluator& operator=(const NumericalEvaluator& other);
    ~NumericalEvaluator();

    int evaluate(int numerical_index, const core::Numerical& numerical, int state_index, const core::State& state, core::PerElementEvaluationCache& caches);

    void clear();
};

}
}

#endif
