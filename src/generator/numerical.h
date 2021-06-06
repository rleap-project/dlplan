#ifndef DLP_SRC_GENERATOR_NUMERICAL_H_
#define DLP_SRC_GENERATOR_NUMERICAL_H_

#include <string>
#include <vector>

#include "feature.h"


namespace dlp {
namespace core {
namespace generator {

class NumericalImpl : public FeatureImpl<int> {
public:
    NumericalImpl(const std::string& repr, std::vector<int>&& state_evaluations)
        : FeatureImpl<int>(repr, std::move(state_evaluations)) { }
    ~NumericalImpl() = default;
};

}
}
}

#endif
