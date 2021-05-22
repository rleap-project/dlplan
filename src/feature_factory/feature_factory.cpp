#include "../../include/dlp/feature_factory.h"


namespace dlp {

FeatureFactory::FeatureFactory()
    : m_pImpl(std::make_unique<FeatureFactoryImpl>(FeatureFactoryImpl())) {
}

void FeatureFactory::add_atom(const std::string &predicate_name,
    Name_Vec &object_names,
    bool constant) {
    m_pImpl->add_atom(predicate_name, object_names, constant);
}

void FeatureFactory::add_atom(const std::string &predicate_name,
    unsigned predicate_idx,
    Name_Vec &object_names,
    Index_Vec &object_idxs,
    bool constant) {
    m_pImpl->add_atom(predicate_name, predicate_idx, object_names, object_idxs, constant);
}


ConceptElement_Ptr FeatureFactory::make_concept_element(const std::string &description) {
    m_pImpl->make_concept_element(description);
}


RoleElement_Ptr FeatureFactory::make_role_element(const std::string &description) {
    m_pImpl->make_role_element(description);
}


NumericalFeature_Ptr FeatureFactory::make_numerical_feature(const std::string &description) {
    m_pImpl->make_numerical_feature(description);
}


BooleanFeature_Ptr FeatureFactory::make_boolean_feature(const std::string &description) {
    m_pImpl->make_boolean_feature(description);
}

}