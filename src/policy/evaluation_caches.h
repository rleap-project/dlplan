#ifndef DLPLAN_SRC_POLICY_EVALUATION_CACHES_H_
#define DLPLAN_SRC_POLICY_EVALUATION_CACHES_H_

#include "per_state_array.h"


namespace dlplan::policy {

class EvaluationCaches {
private:
    PerStateArray<std::vector<int>> m_numerical_feature_evaluations;
    PerStateArray<std::vector<bool>> m_numerical_feature_evaluation_is_cached;

    PerStateArray<std::vector<bool>> m_boolean_feature_evaluations;
    PerStateArray<std::vector<bool>> m_boolean_feature_evaluation_is_cached;

public:
    EvaluationCaches();
};

}

#endif
