#ifndef DLP_INCLUDE_DLP_FEATURE_FACTORY_H_
#define DLP_INCLUDE_DLP_FEATURE_FACTORY_H_

#include "element_factory.h"
#include "../../src/feature_factory/numerical_features/numerical_feature.h"
#include "../../src/feature_factory/boolean_features/boolean_feature.h"
#include "../../src/element_factory/cache.h"
#include "../../src/element_factory/types.h"

namespace dlp {

/**
 * FeatureFactory extends ElementFactory with functionality
 * that allows constructing features.
 * A Feature adds additional layers of composition
 * on top of description logics elements.
 */
class FeatureFactory : public ElementFactory {
private:
    // TODO(dominik): The parser must transform descriptions into canonical representation.
    Cache<std::string, Feature_Ptr> m_feature_cache;
public:
    FeatureFactory();

    /**
     * Returns a pointer to a NumericalFeature if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    NumericalFeature_Ptr make_numerical_feature(const std::string &description);

    /**
     * Returns a pointer to a BooleanFeature if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    BooleanFeature_Ptr make_boolean_feature(const std::string &description);
};

}

#endif
