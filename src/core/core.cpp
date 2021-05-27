#include "../../include/dlp/core.h"

#include "elements/concept.h"
#include "elements/role.h"
#include "elements/numerical.h"
#include "elements/boolean.h"

#include "element_factory.h"
#include "instance_info.h"
#include "atom.h"
#include "state.h"

#include <iostream>


namespace dlp {
namespace core {

InstanceInfo::InstanceInfo() : m_pImpl(std::make_shared<InstanceInfoImpl>()) { }

InstanceInfo::~InstanceInfo() { }

State InstanceInfo::convert_state(const Index_Vec& atom_idxs) {
    return State(m_pImpl->get()->convert_state(*m_pImpl, atom_idxs));
}

Atom InstanceInfo::add_atom(const std::string &predicate_name, const Name_Vec &object_names, bool is_static) {
    return Atom(m_pImpl->get()->add_atom(*m_pImpl, predicate_name, object_names, is_static));
}


Atom::Atom(const AtomImpl& impl) : m_pImpl(impl) { }

Atom::Atom(const Atom& other) : m_pImpl(*(other.m_pImpl)) { }

Atom::~Atom() { }


State::State(StateImpl impl) : m_pImpl(impl) { }

State::State(const State& other) : m_pImpl(*(other.m_pImpl)) {}

State::~State() { }

std::string State::str() const { /* tba */ }


ConceptElement::ConceptElement(element::ConceptElement_Ptr pImpl)
    : m_pImpl(pImpl) { }

ConceptElement::ConceptElement(const ConceptElement& other)
    : m_pImpl(*(other.m_pImpl)) { }

Concepts ConceptElement::evaluate(const State& state) const {
    m_pImpl->get()->evaluate(*(state.m_pImpl));
}

unsigned ConceptElement::complexity() const {
    return m_pImpl->get()->complexity();
}

RoleElement::RoleElement(element::RoleElement_Ptr pImpl)
    : m_pImpl(pImpl) { }

RoleElement::RoleElement(const RoleElement& other)
    : m_pImpl(*(other.m_pImpl)) { }

Roles RoleElement::evaluate(const State& state) const {
    m_pImpl->get()->evaluate(*(state.m_pImpl));
}

unsigned RoleElement::complexity() const {
    return m_pImpl->get()->complexity();
}

NumericalElement::NumericalElement(element::NumericalElement_Ptr pImpl)
    : m_pImpl(pImpl) { }

NumericalElement::NumericalElement(const NumericalElement& other)
    : m_pImpl(*(other.m_pImpl)) { }

int NumericalElement::evaluate(const State& state) const {
    m_pImpl->get()->evaluate(*(state.m_pImpl));
}

unsigned NumericalElement::complexity() const {
    return m_pImpl->get()->complexity();
}

BooleanElement::BooleanElement(element::BooleanElement_Ptr pImpl)
    : m_pImpl(pImpl) { }

BooleanElement::BooleanElement(const BooleanElement& other)
    : m_pImpl(*(other.m_pImpl)) { }

bool BooleanElement::evaluate(const State& state) const {
    m_pImpl->get()->evaluate(*(state.m_pImpl));
}

unsigned BooleanElement::complexity() const {
    return m_pImpl->get()->complexity();
}


ElementFactory::ElementFactory() { }

ElementFactory::~ElementFactory() { }

ConceptElement ElementFactory::parse_concept_element(const InstanceInfo& info, const std::string &description) {
    return ConceptElement(m_pImpl->parse_concept_element(*info.m_pImpl, description));
}

RoleElement ElementFactory::parse_role_element(const InstanceInfo& info, const std::string &description) {
    return RoleElement(m_pImpl->parse_role_element(*info.m_pImpl, description));
}

NumericalElement ElementFactory::parse_numerical_element(const InstanceInfo& info, const std::string &description) {
    return NumericalElement(m_pImpl->parse_numerical_element(*info.m_pImpl, description));
}

BooleanElement ElementFactory::parse_boolean_element(const InstanceInfo& info, const std::string &description) {
    return BooleanElement(m_pImpl->parse_boolean_element(*info.m_pImpl, description));
}

NumericalElement ElementFactory::make_count_element(const InstanceInfo& info, const ConceptElement& element) {
    return NumericalElement(m_pImpl->make_count_element(*info.m_pImpl, *element.m_pImpl));
}

NumericalElement ElementFactory::make_count_element(const InstanceInfo& info, const RoleElement& element) {
    return NumericalElement(m_pImpl->make_count_element(*info.m_pImpl, *element.m_pImpl));
}

ConceptElement ElementFactory::make_primitive_concept_element(const InstanceInfo& info, const std::string& name, unsigned pos) {
    return ConceptElement(m_pImpl->make_primitive_concept_element(*info.m_pImpl, name, pos));
}

}
}
