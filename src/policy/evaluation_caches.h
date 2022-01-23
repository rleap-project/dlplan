#ifndef DLPLAN_SRC_POLICY_EVALUATION_CACHES_H_
#define DLPLAN_SRC_POLICY_EVALUATION_CACHES_H_

#include "per_state_array.h"


namespace dlplan::policy {

template<typename T>
class EvaluationCache {
private:
    PerStateArray<std::vector<T>> m_evaluations;
    PerStateArray<std::vector<bool>> m_is_cached;

public:
    EvaluationCache(std::vector<T> default_value)
    : m_evaluations(default_value), m_is_cached(std::vector<bool>(default_value.size(), false)) { }


};

class EvaluationCaches {
private:
    EvaluationCache<int> m_numerical_cache;
    EvaluationCache<bool> m_boolean_cache;

public:
    EvaluationCaches();
};

}

#endif
