#ifndef DLP_INCLUDE_DLP_FEATURE_FACTORY_H_
#define DLP_INCLUDE_DLP_FEATURE_FACTORY_H_

#include "../../src/feature_factory/types.h"
#include "../../src/feature_factory/feature_factory_impl.h"
#include <memory>

namespace dlp {

/**
 * The FeatureFactory allow the construction of ConceptElement,
 * RoleElement, NumericalFeature, and BooleanFeature from text.
 */
class FeatureFactory {
public:
    FeatureFactory();

    void add_atom(const std::string &predicate_name,
        Name_Vec &object_names,
        bool constant=false);

    void add_atom(const std::string &predicate_name,
        unsigned predicate_idx,
        Name_Vec &object_names,
        Index_Vec &object_idxs,
        bool constant=false);

    /**
     * Returns a pointer to a ConceptElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    ConceptElement_Ptr make_concept_element(const std::string &description);

    /**
     * Returns a pointer to a RoleElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    RoleElement_Ptr make_role_element(const std::string &description);

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
private:
    std::unique_ptr<FeatureFactoryImpl> m_pImpl;
};

}

#endif
