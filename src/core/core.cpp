#include "../../include/dlp/core.h"

#include "elements/concept.h"
#include "elements/role.h"
#include "elements/numerical.h"
#include "elements/boolean.h"


namespace dlp {
namespace core {

ElementFactory::ElementFactory()
    : m_pImpl(std::make_unique<ElementFactoryImpl>(ElementFactoryImpl())) {
}

int ElementFactory::add_atom(const std::string &predicate_name, const Name_Vec &object_names, bool goal) {

}

ConceptElement ElementFactory::make_concept_element(const std::string &description) {
    return ConceptElement(m_pImpl->make_concept_element(description));
}

RoleElement ElementFactory::make_role_element(const std::string &description) {
    return RoleElement(m_pImpl->make_role_element(description));
}

NumericalElement ElementFactory::make_numerical_element(const std::string &description) {
    return NumericalElement(m_pImpl->make_numerical_element(description));
}

BooleanElement ElementFactory::make_boolean_element(const std::string &description) {
    return BooleanElement(m_pImpl->make_boolean_element(description));
}

ConceptElement::ConceptElement(element::ConceptElement_Ptr pImpl)
    : Element<Concepts>(), m_pImpl(pImpl) {
}

Concepts ConceptElement::evaluate(const Index_Vec& atoms) const {
    m_pImpl->evaluate(atoms);
}

unsigned ConceptElement::complexity() const {
    return m_pImpl->complexity();
}

RoleElement::RoleElement(element::RoleElement_Ptr pImpl)
    : Element<Roles>(), m_pImpl(pImpl) {
}

Roles RoleElement::evaluate(const Index_Vec& atoms) const {
    m_pImpl->evaluate(atoms);
}

unsigned RoleElement::complexity() const {
    return m_pImpl->complexity();
}

NumericalElement::NumericalElement(element::NumericalElement_Ptr pImpl)
    : Element<int>(), m_pImpl(pImpl) {
}

int NumericalElement::evaluate(const Index_Vec& atoms) const {
    m_pImpl->evaluate(atoms);
}

unsigned NumericalElement::complexity() const {
    return m_pImpl->complexity();
}

BooleanElement::BooleanElement(element::BooleanElement_Ptr pImpl)
    : Element<bool>(), m_pImpl(pImpl) {
}

bool BooleanElement::evaluate(const Index_Vec& atoms) const {
    m_pImpl->evaluate(atoms);
}

unsigned BooleanElement::complexity() const {
    return m_pImpl->complexity();
}

}
}
