#ifndef DLPLAN_SRC_EVALUATOR_BOOLEAN_EVALUATOR_H_
#define DLPLAN_SRC_EVALUATOR_BOOLEAN_EVALUATOR_H_

#include "../utils/per_index_bitset.h"

#include "../../include/dlplan/core.h"


namespace dlplan::evaluator {

class BooleanEvaluatorImpl {
private:
    utils::PerIndexBitset m_data;

public:
    explicit BooleanEvaluatorImpl(int num_features) : m_data(std::vector<bool>(2 * std::max(1, num_features), false)) { }

    bool evaluate(int boolean_index, const core::Boolean& boolean, int state_index, const core::State& state, core::EvaluationCaches& caches) {
        auto view = m_data[state_index];
        int start = 2 * boolean_index;
        if (!view.test(start)) {
            // Since evaluation is initialized to false,
            // we must only set if the element evaluates to true.
            // If it evaluates to false then marking as cached is sufficient.
            if (boolean.evaluate(state, caches))
                view.set(start + 1);
            view.set(start);
        }
        return view.test(start + 1);
    }
};

}

#endif
