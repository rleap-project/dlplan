#include "../../include/dlp/core.h"

#include "elements/concept.h"
#include "elements/role.h"
#include "elements/numerical.h"
#include "elements/boolean.h"

#include "element_factory.h"
#include "instance_info.h"
#include "atom.h"
#include "state.h"
#include "predicate.h"

#include <iostream>


namespace dlp {
namespace core {

InstanceInfo::InstanceInfo() : m_pImpl(std::make_shared<InstanceInfoImpl>(InstanceInfoImpl())) { }

InstanceInfo::~InstanceInfo() { }

State InstanceInfo::parse_state(const Name_Vec& atom_names) const {
    return State(*m_pImpl, m_pImpl->get()->parse_state(*m_pImpl, atom_names));
}

State InstanceInfo::convert_state(const Index_Vec& atom_idxs) const {
    return State(*m_pImpl, m_pImpl->get()->convert_state(*m_pImpl, atom_idxs));
}

Atom InstanceInfo::add_atom(const std::string& atom_name, const std::string &predicate_name, const Name_Vec &object_names, bool is_static) {
    return Atom(m_pImpl->get()->add_atom(atom_name, predicate_name, object_names, is_static));
}

std::vector<Predicate> InstanceInfo::predicates() const {
    std::vector<Predicate> predicates;
    for (const PredicateImpl& predicate_impl : m_pImpl->get()->predicates()) {
        predicates.push_back(Predicate(predicate_impl));
    }
    return predicates;
}


Predicate::Predicate(const PredicateImpl& impl) : m_pImpl(impl) { }

Predicate::Predicate(const Predicate& other) : m_pImpl(*other.m_pImpl) { }

Predicate::~Predicate() {}

const std::string& Predicate::name() const {
    return m_pImpl->m_predicate_name;
}

unsigned Predicate::arity() const {
    return m_pImpl->m_arity;
}


Atom::Atom(const AtomImpl& impl) : m_pImpl(impl) { }

Atom::Atom(const Atom& other) : m_pImpl(*other.m_pImpl) { }

Atom::~Atom() { }


State::State(std::shared_ptr<InstanceInfoImpl> parent, StateImpl impl) : m_parent(parent), m_pImpl(impl) { }

State::State(const State& other) : m_parent(other.m_parent), m_pImpl(*other.m_pImpl) {}

State::~State() { }

std::string State::str() const { /* tba */ }


ConceptElement::ConceptElement(std::shared_ptr<InstanceInfoImpl> parent, element::ConceptElement_Ptr pImpl)
    : Element<Concepts>(parent), m_pImpl(pImpl) { }

ConceptElement::ConceptElement(const ConceptElement& other)
    : Element<Concepts>(other.m_parent), m_pImpl(*other.m_pImpl) { }

Concepts ConceptElement::evaluate(const State& state) const {
    if (m_parent != state.m_parent) {
        throw std::invalid_argument("ConceptElement::evaluate - instance information between state and element do not match.");
    }
    return m_pImpl->get()->evaluate(*state.m_pImpl);
}

unsigned ConceptElement::complexity() const {
    return m_pImpl->get()->complexity();
}

std::string ConceptElement::repr() const {
    return m_pImpl->get()->repr();
}

RoleElement::RoleElement(std::shared_ptr<InstanceInfoImpl> parent, element::RoleElement_Ptr pImpl)
    : Element<Roles>(parent), m_pImpl(pImpl) { }

RoleElement::RoleElement(const RoleElement& other)
    : Element<Roles>(other.m_parent), m_pImpl(*other.m_pImpl) { }

Roles RoleElement::evaluate(const State& state) const {
    if (m_parent != state.m_parent) {
        throw std::invalid_argument("RoleElement::evaluate - instance information between state and element do not match.");
    }
    return m_pImpl->get()->evaluate(*state.m_pImpl);
}

unsigned RoleElement::complexity() const {
    return m_pImpl->get()->complexity();
}

std::string RoleElement::repr() const {
    return m_pImpl->get()->repr();
}


NumericalElement::NumericalElement(std::shared_ptr<InstanceInfoImpl> parent, element::NumericalElement_Ptr pImpl)
    : Element<int>(parent), m_pImpl(pImpl) { }

NumericalElement::NumericalElement(const NumericalElement& other)
    : Element<int>(other.m_parent), m_pImpl(*other.m_pImpl) { }

int NumericalElement::evaluate(const State& state) const {
    if (m_parent != state.m_parent) {
        throw std::invalid_argument("NumericalElement::evaluate - instance information between state and element do not match.");
    }
    return m_pImpl->get()->evaluate(*(state.m_pImpl));
}

unsigned NumericalElement::complexity() const {
    return m_pImpl->get()->complexity();
}

std::string NumericalElement::repr() const {
    return m_pImpl->get()->repr();
}


BooleanElement::BooleanElement(std::shared_ptr<InstanceInfoImpl> parent, element::BooleanElement_Ptr pImpl)
    : Element<bool>(parent), m_pImpl(pImpl) { }

BooleanElement::BooleanElement(const BooleanElement& other)
    : Element<bool>(other.m_parent), m_pImpl(*other.m_pImpl) { }

bool BooleanElement::evaluate(const State& state) const {
    if (m_parent != state.m_parent) {
        throw std::invalid_argument("BooleanElement::evaluate - instance information between state and element do not match.");
    }
    return m_pImpl->get()->evaluate(*state.m_pImpl);
}

unsigned BooleanElement::complexity() const {
    return m_pImpl->get()->complexity();
}

std::string BooleanElement::repr() const {
    return m_pImpl->get()->repr();
}


ElementFactory::ElementFactory() { }

ElementFactory::~ElementFactory() { }

ConceptElement ElementFactory::parse_concept_element(const InstanceInfo& info, const std::string &description) {
    return ConceptElement(*info.m_pImpl, m_pImpl->parse_concept_element(**info.m_pImpl, description));
}

RoleElement ElementFactory::parse_role_element(const InstanceInfo& info, const std::string &description) {
    return RoleElement(*info.m_pImpl, m_pImpl->parse_role_element(**info.m_pImpl, description));
}

NumericalElement ElementFactory::parse_numerical_element(const InstanceInfo& info, const std::string &description) {
    return NumericalElement(*info.m_pImpl, m_pImpl->parse_numerical_element(**info.m_pImpl, description));
}

BooleanElement ElementFactory::parse_boolean_element(const InstanceInfo& info, const std::string &description) {
    return BooleanElement(*info.m_pImpl, m_pImpl->parse_boolean_element(**info.m_pImpl, description));
}

NumericalElement ElementFactory::make_count_element(const InstanceInfo& info, const ConceptElement& element) {
    return NumericalElement(*info.m_pImpl, m_pImpl->make_count_element(**info.m_pImpl, *element.m_pImpl));
}

NumericalElement ElementFactory::make_count_element(const InstanceInfo& info, const RoleElement& element) {
    return NumericalElement(*info.m_pImpl, m_pImpl->make_count_element(**info.m_pImpl, *element.m_pImpl));
}

ConceptElement ElementFactory::make_primitive_concept_element(const InstanceInfo& info, const std::string& name, unsigned pos) {
    return ConceptElement(*info.m_pImpl, m_pImpl->make_primitive_concept_element(**info.m_pImpl, name, pos));
}

}
}
