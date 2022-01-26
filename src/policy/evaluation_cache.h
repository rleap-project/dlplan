#ifndef DLPLAN_SRC_POLICY_EVALUATION_CACHE_H_
#define DLPLAN_SRC_POLICY_EVALUATION_CACHE_H_

#include "per_state_array.h"
#include "per_state_bitset.h"


namespace dlplan::policy {

class BooleanEvaluationCache {
private:
    PerStateBitset m_data;

public:
    BooleanEvaluationCache(int num_features)
    : m_data(std::vector<bool>(2 * num_features, false)) { }

    bool get_feature_evaluation(const BooleanFeature& feature, const State& state) {
        int index = feature.get_index();
        auto view = m_data[state];
        int start = 2 * index;
        if (!view.test(start)) {
            // Since evaluation is initialized to false,
            // we must only set if the element evaluates to true.
            // If it evaluates to false then marking as cached is sufficient.
            if (feature.get_boolean().evaluate(state.get_state()))
                view.set(start + 1);
            view.set(start);
        }
        return view.test(start + 1);
    }
};

class NumericalEvaluationCache {
private:

    PerStateArray<int> m_data;

public:
    NumericalEvaluationCache(int num_features)
    : m_data(std::vector<int>(num_features, -1)) { }

    int get_feature_evaluation(const NumericalFeature& feature, const State& state) {
        int index = feature.get_index();
        auto view = m_data[state];
        // -1 represents that the value is not cached.
        if (view[index] == -1) {
            view[index] = feature.get_numerical().evaluate(state.get_state());
        }
        return view[index];
    }
};


class EvaluationCaches {
private:
    BooleanEvaluationCache m_boolean_cache;
    NumericalEvaluationCache m_numerical_cache;

public:
    // HACK: we set minimum size to 1 because of division by zero
    EvaluationCaches(int num_boolean_features, int num_numerical_features)
    : m_boolean_cache(std::max(1, num_boolean_features)),
      m_numerical_cache(std::max(1, num_numerical_features)) { }

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
