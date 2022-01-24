#ifndef DLPLAN_SRC_POLICY_EVALUATION_CACHE_H_
#define DLPLAN_SRC_POLICY_EVALUATION_CACHE_H_

#include "per_state_array.h"
#include "per_state_bitset.h"


namespace dlplan::policy {

class BooleanEvaluationCache {
private:
    PerStateBitset m_evaluations;
    PerStateBitset m_is_cached;

public:
    BooleanEvaluationCache(int num_features)
    : m_evaluations(std::vector<bool>(num_features, false)),
      m_is_cached(std::vector<bool>(num_features, false)) { }

    PerStateBitset& get_evaluations() {
        return m_evaluations;
    }

    const PerStateBitset& get_evaluations() const {
        return m_evaluations;
    }

    PerStateBitset& get_is_cached() {
        return m_is_cached;
    }

    const PerStateBitset& get_is_cached() const {
        return m_is_cached;
    }
};

class NumericalEvaluationCache {
private:
    PerStateArray<int> m_evaluations;
    PerStateBitset m_is_cached;

public:
    NumericalEvaluationCache(int num_features)
    : m_evaluations(std::vector<int>(num_features, -1)),
      m_is_cached(std::vector<bool>(num_features, false)) { }

    PerStateArray<int>& get_evaluations() {
        return m_evaluations;
    }

    const PerStateArray<int>& get_evaluations() const {
        return m_evaluations;
    }

    PerStateBitset& get_is_cached() {
        return m_is_cached;
    }

    const PerStateBitset& get_is_cached() const {
        return m_is_cached;
    }
};


class EvaluationCaches {
private:
    BooleanEvaluationCache m_boolean_cache;
    NumericalEvaluationCache m_numerical_cache;

public:
    EvaluationCaches(int num_boolean_features, int num_numerical_features)
    : m_boolean_cache(num_boolean_features),
      m_numerical_cache(num_numerical_features) {
      }

    BooleanEvaluationCache& get_boolean_cache() {
        return m_boolean_cache;
    }

    const BooleanEvaluationCache& get_boolean_cache() const {
        return m_boolean_cache;
    }

    NumericalEvaluationCache& get_numerical_cache() {
        return m_numerical_cache;
    }

    const NumericalEvaluationCache& get_numerical_cache() const {
        return m_numerical_cache;
    }
};

}

#endif
