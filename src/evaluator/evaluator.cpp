#include "../../include/dlplan/evaluator.h"
#include "../../include/dlplan/core.h"


namespace dlplan::evaluator {

EvaluationCache::EvaluationCache(int num_booleans, int num_numericals)
  : m_boolean_denots_cache(std::vector<bool>(2 * std::max(1, num_booleans), false)),
    m_numerical_denots_cache(std::vector<int>(std::max(1, num_numericals), -1)) { }

EvaluationCache::EvaluationCache(const EvaluationCache& other) = default;

EvaluationCache& EvaluationCache::operator=(const EvaluationCache& other) = default;

EvaluationCache::EvaluationCache(EvaluationCache&& other) = default;

EvaluationCache& EvaluationCache::operator=(EvaluationCache&& other) = default;

bool EvaluationCache::retrieve_or_evaluate(int boolean_idx, const core::Boolean& boolean, EvaluationContext& context) {
    assert(this == &context.cache);
    auto view = m_boolean_denots_cache[context.state_idx];
    int start = 2 * boolean_idx;
    if (!view.test(start)) {
        // Since evaluation is initialized to false,
        // we must only set if the element evaluates to true.
        // If it evaluates to false then marking as cached is sufficient.
        if (boolean.evaluate(context.state))
            view.set(start + 1);
        view.set(start);
    }
    return view.test(start + 1);
}

int EvaluationCache::retrieve_or_evaluate(int numerical_idx, const core::Numerical& numerical, EvaluationContext& context) {
    assert(this == &context.cache);
    auto view = m_numerical_denots_cache[context.state_idx];
    // -1 represents that the value is not cached.
    if (view[numerical_idx] == -1) {
        view[numerical_idx] = numerical.evaluate(context.state);
    }
    return view[numerical_idx];
}

EvaluationContext::EvaluationContext(int state_idx, const core::State& state, EvaluationCache& cache)
    : state_idx(state_idx), state(state), cache(cache) {}

}