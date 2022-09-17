#ifndef DLPLAN_INCLUDE_DLPLAN_EVALUATOR_H_
#define DLPLAN_INCLUDE_DLPLAN_EVALUATOR_H_

#include "core.h"

#include "utils/per_index_array.h"
#include "utils/per_index_bitset.h"


namespace dlplan::evaluator {
class EvaluationContext;

/**
 * Provides a cache entry for the result of evaluating an element on a given state.
 *
 * This cache is useful if one wants to evaluate Boolean and Numericals
 * with the same state multiple times.
 *
 * An example usage is a Policy where a target state
 * becomes the new source state if a transition is classified as good.
 */
class EvaluationCache {
private:
    utils::PerIndexBitset m_boolean_denots_cache;
    utils::PerIndexArray<int> m_numerical_denots_cache;

public:
    EvaluationCache(int num_booleans, int num_numericals);
    ~EvaluationCache();

    bool retrieve_or_evaluate(const core::Boolean& boolean, const core::State& state);
    int retrieve_or_evaluate(const core::Numerical& numerical, const core::State& state);
};

}

#endif
