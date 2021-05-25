#include "../../include/dlp/core.h"

#include "elements/concept.h"
#include "elements/role.h"
#include "elements/numerical.h"
#include "elements/boolean.h"


namespace dlp {
namespace core {

InstanceInfo::InstanceInfo()
    : m_pImpl(std::make_shared<InstanceInfoImpl>()) {
}

int InstanceInfo::add_atom(const std::string &predicate_name, const Name_Vec &object_names, bool goal) {

}

ElementFactory::ElementFactory()
    : m_pImpl(std::make_unique<ElementFactoryImpl>()) {
}

ConceptElement ElementFactory::make_concept_element(const InstanceInfo& info, const std::string &description) {
    return ConceptElement(info, m_pImpl->make_concept_element(*info.m_pImpl, description));
}

RoleElement ElementFactory::make_role_element(const InstanceInfo& info, const std::string &description) {
    return RoleElement(info, m_pImpl->make_role_element(*info.m_pImpl, description));
}

NumericalElement ElementFactory::make_numerical_element(const InstanceInfo& info, const std::string &description) {
    return NumericalElement(info, m_pImpl->make_numerical_element(*info.m_pImpl, description));
}

BooleanElement ElementFactory::make_boolean_element(const InstanceInfo& info, const std::string &description) {
    return BooleanElement(info, m_pImpl->make_boolean_element(*info.m_pImpl, description));
}

ConceptElement::ConceptElement(const InstanceInfo& info, element::ConceptElement_Ptr pImpl)
    : Element<Concepts>(info), m_pImpl(pImpl) {
}

Concepts ConceptElement::evaluate(const Index_Vec& atoms) const {
    m_pImpl->evaluate(*m_info.m_pImpl, atoms);
}

unsigned ConceptElement::complexity() const {
    return m_pImpl->complexity();
}

RoleElement::RoleElement(const InstanceInfo& info, element::RoleElement_Ptr pImpl)
    : Element<Roles>(info), m_pImpl(pImpl) {
}

Roles RoleElement::evaluate(const Index_Vec& atoms) const {
    m_pImpl->evaluate(*m_info.m_pImpl, atoms);
}

unsigned RoleElement::complexity() const {
    return m_pImpl->complexity();
}

NumericalElement::NumericalElement(const InstanceInfo& info, element::NumericalElement_Ptr pImpl)
    : Element<int>(info), m_pImpl(pImpl) {
}

int NumericalElement::evaluate(const Index_Vec& atoms) const {
    m_pImpl->evaluate(*m_info.m_pImpl, atoms);
}

unsigned NumericalElement::complexity() const {
    return m_pImpl->complexity();
}

BooleanElement::BooleanElement(const InstanceInfo& info, element::BooleanElement_Ptr pImpl)
    : Element<bool>(info), m_pImpl(pImpl) {
}

bool BooleanElement::evaluate(const Index_Vec& atoms) const {
    m_pImpl->evaluate(*m_info.m_pImpl, atoms);
}

unsigned BooleanElement::complexity() const {
    return m_pImpl->complexity();
}

}
}
