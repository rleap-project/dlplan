#ifndef DLP_SRC_GENERATOR_BOOLEAN_H_
#define DLP_SRC_GENERATOR_BOOLEAN_H_

#include <string>
#include <vector>

#include "feature.h"


namespace dlp {
namespace generator {

class BooleanImpl : public FeatureImpl<bool> {
public:
    BooleanImpl(const std::string& repr, std::vector<bool>&& state_evaluations)
        : FeatureImpl<bool>(repr, std::move(state_evaluations)) { }
    ~BooleanImpl() = default;
};

}
}

#endif
