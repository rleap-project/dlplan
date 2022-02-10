#ifndef DLPLAN_SRC_EVALUATOR_NUMERICAL_EVALUATOR_H_
#define DLPLAN_SRC_EVALUATOR_NUMERICAL_EVALUATOR_H_

#include "../utils/per_index_bitset.h"

#include "../../include/dlplan/core.h"


namespace dlplan::evaluator {

class NumericalEvaluatorImpl {
private:
    utils::PerIndexArray<int> m_data;

public:
    explicit NumericalEvaluatorImpl(int num_features)
    : m_data(std::vector<int>(std::max(1, num_features), -1)) { }

    int evaluate(int numerical_index, const core::Numerical& numerical, int state_index, const core::State& state) {
        auto view = m_data[state_index];
        // -1 represents that the value is not cached.
        if (view[numerical_index] == -1) {
            view[numerical_index] = numerical.evaluate(state);
        }
        return view[numerical_index];
    }
};


}

#endif
