#include "../../include/dlp/element_factory.h"


namespace dlp {

ElementFactory::ElementFactory()
    : m_pImpl(std::make_unique<ElementFactoryImpl>(ElementFactoryImpl())) {
}

void ElementFactory::add_atom(const std::string &predicate_name, const Name_Vec &object_names) {
    m_pImpl->add_atom(predicate_name, object_names);
}

void ElementFactory::set_constant_atoms(const Index_Vec& constant_atom_idxs) {
    m_pImpl->set_constant_atoms(constant_atom_idxs);
}

void ElementFactory::set_goal_atoms(const Index_Vec& goal_atom_idxs) {
    m_pImpl->set_goal_atoms(goal_atom_idxs);
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