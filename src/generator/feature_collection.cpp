#include "feature_collection.h"


namespace dlplan::generator {

void FeatureCollectionImpl::add_numerical_feature(Numerical&& numerical_feature) {
    m_numerical_features.push_back(std::move(numerical_feature));
}

void FeatureCollectionImpl::add_boolean_feature(Boolean&& boolean_feature) {
    m_boolean_features.push_back(std::move(boolean_feature));
}

const NumericalFeatures& FeatureCollectionImpl::get_numerical_features() const {
    return m_numerical_features;
}

const BooleanFeatures& FeatureCollectionImpl::get_boolean_features() const {
    return m_boolean_features;
}

}
