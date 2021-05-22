#include "feature_factory_impl.h"
#include "lisp/parser.h"
#include "concept_elements/concept_element.h"
#include "role_elements/role_element.h"

namespace dlp {

FeatureFactoryImpl::FeatureFactoryImpl() {
}

void FeatureFactoryImpl::add_atom(const std::string &predicate_name,
    Name_Vec &object_names,
    bool constant) {

}

void FeatureFactoryImpl::add_atom(const std::string &predicate_name,
    unsigned predicate_idx,
    Name_Vec &object_names,
    Index_Vec &object_idxs,
    bool constant) {

}


ConceptElement_Ptr FeatureFactoryImpl::make_concept_element(const std::string &description) {
    lisp::Expression_Ptr expression = lisp::Parser().parse(description);
    return std::make_shared<ConceptElement>(ConceptElement());
}

RoleElement_Ptr FeatureFactoryImpl::make_role_element(const std::string &description) {
    return std::make_shared<RoleElement>(RoleElement());
}

NumericalFeature_Ptr FeatureFactoryImpl::make_numerical_feature(const std::string &description) {

}

BooleanFeature_Ptr FeatureFactoryImpl::make_boolean_feature(const std::string &description) {

}

}
