#include "../../include/dlp/core.h"

#include "elements/concept.h"
#include "elements/role.h"
#include "elements/numerical.h"
#include "elements/boolean.h"


namespace dlp {
namespace core {

State::State(const InstanceInfo& info, const Index_Vec& atoms)
    : m_info(info), m_atoms(atoms) {
}

State::State(const InstanceInfo& info, Index_Vec&& atoms)
    : m_info(info), m_atoms(std::move(atoms)) {
}

InstanceInfo State::info() const {
    return m_info;
}

const Index_Vec& State::atoms() const {
    return m_atoms;
}

InstanceInfo::InstanceInfo()
    : m_pImpl(std::make_shared<InstanceInfoImpl>()) {
}

int InstanceInfo::add_atom(const std::string &predicate_name, const Name_Vec &object_names) {
}

bool InstanceInfo::operator==(const InstanceInfo& other) {
    return m_pImpl == other.m_pImpl;
}

bool InstanceInfo::operator!=(const InstanceInfo& other) {
    return !(*this == other);
}

std::shared_ptr<InstanceInfoImpl> InstanceInfo::impl() const {
    return m_pImpl;
}

State InstanceInfo::parse_state(/* tba */) const {
    return State(*this, {});
}

ElementFactory::ElementFactory()
    : m_pImpl(std::make_unique<ElementFactoryImpl>()) {
}

ConceptElement ElementFactory::parse_concept_element(const InstanceInfo& info, const std::string &description) {
    return ConceptElement(info, m_pImpl->parse_concept_element(*info.impl(), description));
}

RoleElement ElementFactory::parse_role_element(const InstanceInfo& info, const std::string &description) {
    return RoleElement(info, m_pImpl->parse_role_element(*info.impl(), description));
}

NumericalElement ElementFactory::parse_numerical_element(const InstanceInfo& info, const std::string &description) {
    return NumericalElement(info, m_pImpl->parse_numerical_element(*info.impl(), description));
}

BooleanElement ElementFactory::parse_boolean_element(const InstanceInfo& info, const std::string &description) {
    return BooleanElement(info, m_pImpl->parse_boolean_element(*info.impl(), description));
}

ConceptElement::ConceptElement(const InstanceInfo& info, element::ConceptElement_Ptr pImpl)
    : Element<Concepts>(info), m_pImpl(pImpl) {
}

Concepts ConceptElement::evaluate(const State& state) const {
    if (info() != state.info()) {
        throw std::invalid_argument("ConceptElement::evaluate - instance information between state and element do not match.");
    }
    m_pImpl->evaluate(*m_info.impl(), state);
}

unsigned ConceptElement::complexity() const {
    return m_pImpl->complexity();
}

RoleElement::RoleElement(const InstanceInfo& info, element::RoleElement_Ptr pImpl)
    : Element<Roles>(info), m_pImpl(pImpl) {
}

Roles RoleElement::evaluate(const State& state) const {
    if (info() != state.info()) {
        throw std::invalid_argument("RoleElement::evaluate - instance information between state and element do not match.");
    }
    m_pImpl->evaluate(*m_info.impl(), state);
}

unsigned RoleElement::complexity() const {
    return m_pImpl->complexity();
}

NumericalElement::NumericalElement(const InstanceInfo& info, element::NumericalElement_Ptr pImpl)
    : Element<int>(info), m_pImpl(pImpl) {
}

int NumericalElement::evaluate(const State& state) const {
    if (info() != state.info()) {
        throw std::invalid_argument("NumericalElement::evaluate - instance information between state and element do not match.");
    }
    m_pImpl->evaluate(*m_info.impl(), state);
}

unsigned NumericalElement::complexity() const {
    return m_pImpl->complexity();
}

BooleanElement::BooleanElement(const InstanceInfo& info, element::BooleanElement_Ptr pImpl)
    : Element<bool>(info), m_pImpl(pImpl) {
}

bool BooleanElement::evaluate(const State& state) const {
    if (info() != state.info()) {
        throw std::invalid_argument("BooleanElement::evaluate - instance information between state and element do not match.");
    }
    m_pImpl->evaluate(*m_info.impl(), state);
}

unsigned BooleanElement::complexity() const {
    return m_pImpl->complexity();
}

}
}
