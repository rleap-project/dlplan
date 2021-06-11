#ifndef DLP_SRC_GENERATOR_BOOLEAN_H_
#define DLP_SRC_GENERATOR_BOOLEAN_H_

#include <string>
#include <vector>

#include "feature.h"


namespace dlp::generator {

class BooleanImpl : public FeatureImpl<bool> {
public:
    BooleanImpl(const std::string& repr, const std::vector<bool>& state_evaluations)
        : FeatureImpl<bool>(repr, state_evaluations) { }
    ~BooleanImpl() = default;
};

}

#endif
