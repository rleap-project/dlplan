#include "../../include/dlp/core.h"


namespace dlp {
namespace core {

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
    return m_pImpl->make_concept_element(description);
}

RoleElement_Ptr ElementFactory::make_role_element(const std::string &description) {
    return m_pImpl->make_role_element(description);
}

NumericalElement_Ptr ElementFactory::make_numerical_element(const std::string &description) {
    return m_pImpl->make_numerical_element(description);
}

BooleanElement_Ptr ElementFactory::make_boolean_element(const std::string &description) {
    return m_pImpl->make_boolean_element(description);
}

ConceptElement::ConceptElement(ElementFactory& factory, const std::string& description)
    : Element<Concepts>(), m_pImpl(factory.make_concept_element(description)) {
}

Concepts ConceptElement::evaluate(const Index_Vec& atoms) const {
    m_pImpl->evaluate(atoms);
}

unsigned ConceptElement::complexity() const {
    return m_pImpl->complexity();
}

RoleElement::RoleElement(ElementFactory& factory, const std::string& description)
    : Element<Roles>(), m_pImpl(factory.make_role_element(description)) {
}

Roles RoleElement::evaluate(const Index_Vec& atoms) const {
    m_pImpl->evaluate(atoms);
}

unsigned RoleElement::complexity() const {
    return m_pImpl->complexity();
}

NumericalElement::NumericalElement(ElementFactory& factory, const std::string& description)
    : Element<int>(), m_pImpl(factory.make_numerical_element(description)) {
}

int NumericalElement::evaluate(const Index_Vec& atoms) const {
    m_pImpl->evaluate(atoms);
}

unsigned NumericalElement::complexity() const {
    return m_pImpl->complexity();
}

BooleanElement::BooleanElement(ElementFactory& factory, const std::string& description)
    : Element<bool>(), m_pImpl(factory.make_boolean_element(description)) {
}

bool BooleanElement::evaluate(const Index_Vec& atoms) const {
    m_pImpl->evaluate(atoms);
}

unsigned BooleanElement::complexity() const {
    return m_pImpl->complexity();
}

}
}
