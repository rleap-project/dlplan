#ifndef DLPLAN_SRC_GENERATOR_FEATURE_COLLECTION_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_COLLECTION_H_

#include <vector>

#include "../../include/dlplan/generator.h"


namespace dlplan::generator {

class FeatureCollectionImpl {
private:
    NumericalFeatures m_numerical_features;
    BooleanFeatures m_boolean_features;

public:
    FeatureCollectionImpl() = default;
    ~FeatureCollectionImpl() = default;

    void add_numerical_feature(Numerical&& numerical_feature);
    void add_boolean_feature(Boolean&& boolean_feature);

    const NumericalFeatures& get_numerical_features() const;
    const BooleanFeatures& get_boolean_features() const;
};

}

#endif
