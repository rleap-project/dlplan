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

bool EvaluationCache::retrieve_or_evaluate(const core::Boolean& boolean, EvaluationContext& context) {
    assert(this == &context.cache);
    auto view = m_boolean_denots_cache[context.state.get_index()];
    int start = 2 * boolean.get_index();
    //view.reset(start);
    //view.reset(start+1);
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

int EvaluationCache::retrieve_or_evaluate(const core::Numerical& numerical, EvaluationContext& context) {
    assert(this == &context.cache);
    auto view = m_numerical_denots_cache[context.state.get_index()];
    // -1 represents that the value is not cached.
    int& value = view[numerical.get_index()];
    //value = -1;
    if (value == -1) {
        value = numerical.evaluate(context.state);
    }
    return value;
}

EvaluationContext::EvaluationContext(const core::State& state, EvaluationCache& cache)
    : state(state), cache(cache) {
        if (state.get_index() < 0 || state.get_index() < 0) {
            throw std::runtime_error("EvaluationContext::EvaluationContext: state_idx index cannot be negative.");
        }
    }
}
