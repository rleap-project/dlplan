#include "../../include/dlp/element_factory.h"


namespace dlp {

ElementFactory::ElementFactory()
    : m_pImpl(std::make_unique<ElementFactoryImpl>(ElementFactoryImpl())) {
}

void ElementFactory::add_atom(const std::string &predicate_name,
    Name_Vec &object_names,
    bool constant) {
    m_pImpl->add_atom(predicate_name, object_names, constant);
}

void ElementFactory::add_atom(const std::string &predicate_name,
    unsigned predicate_idx,
    Name_Vec &object_names,
    Index_Vec &object_idxs,
    bool constant) {
    m_pImpl->add_atom(predicate_name, predicate_idx, object_names, object_idxs, constant);
}


ConceptElement_Ptr ElementFactory::make_concept_element(const std::string &description) {
    m_pImpl->make_concept_element(description);
}


RoleElement_Ptr ElementFactory::make_role_element(const std::string &description) {
    m_pImpl->make_role_element(description);
}


NumericalElement_Ptr ElementFactory::make_numerical_element(const std::string &description) {
    m_pImpl->make_numerical_element(description);
}


BooleanElement_Ptr ElementFactory::make_boolean_element(const std::string &description) {
    m_pImpl->make_boolean_element(description);
}

}