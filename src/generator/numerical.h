#ifndef DLPLAN_SRC_GENERATOR_NUMERICAL_H_
#define DLPLAN_SRC_GENERATOR_NUMERICAL_H_

#include <string>
#include <vector>

#include "feature.h"


namespace dlplan::generator {

class NumericalImpl : public FeatureImpl<int> {
public:
    NumericalImpl(const std::string& repr, const std::vector<int>& state_evaluations)
        : FeatureImpl<int>(repr, state_evaluations) { }
    ~NumericalImpl() = default;
};

}

#endif
