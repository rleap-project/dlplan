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

State InstanceInfo::convert_state(const std::vector<Atom>& atoms) const {
    std::vector<AtomImpl> atoms_impl;
    atoms_impl.reserve(atoms.size());
    for (const auto& atom : atoms) {
        if (atom.m_parent != *m_pImpl) {
            throw std::runtime_error("InstanceInfo::parse_state - atom ("s + atom.atom_name() + ") does not belong to the instance");
        }
        atoms_impl.push_back(*atom.m_pImpl);
    }
    return State(*m_pImpl, m_pImpl->get()->convert_state(*m_pImpl, atoms_impl));
}

State InstanceInfo::convert_state(const Index_Vec& atom_idxs) const {
    return State(*m_pImpl, m_pImpl->get()->convert_state(*m_pImpl, atom_idxs));
}

Atom InstanceInfo::add_atom(const std::string &predicate_name, const Name_Vec &object_names, bool is_static) {
    return Atom(*m_pImpl, m_pImpl->get()->add_atom(predicate_name, object_names, is_static));
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


Atom::Atom(std::shared_ptr<InstanceInfoImpl> parent, AtomImpl&& impl) : m_parent(parent), m_pImpl(std::move(impl)) { }

Atom::Atom(const Atom& other) : m_parent(other.m_parent), m_pImpl(*other.m_pImpl) { }

Atom::~Atom() { }

int Atom::atom_idx() const {
    return m_pImpl->m_atom_idx;
}

const std::string& Atom::atom_name() const {
    return m_pImpl->m_atom_name;
}


State::State(std::shared_ptr<InstanceInfoImpl> parent, StateImpl&& impl) : m_parent(parent), m_pImpl(std::move(impl)) { }

State::State(const State& other) : m_parent(other.m_parent), m_pImpl(*other.m_pImpl) {}

State::~State() { }

std::string State::str() const { /* tba */ }


ConceptElement::ConceptElement(element::ConceptElement_Ptr pImpl)
    : m_pImpl(pImpl) { }

ConceptElement::ConceptElement(const ConceptElement& other)
    : m_pImpl(*other.m_pImpl) { }

Concepts ConceptElement::evaluate(const State& state) const {
    return m_pImpl->get()->evaluate(*state.m_pImpl);
}

unsigned ConceptElement::complexity() const {
    return m_pImpl->get()->complexity();
}

std::string ConceptElement::repr() const {
    return m_pImpl->get()->repr();
}

RoleElement::RoleElement(element::RoleElement_Ptr pImpl)
    : m_pImpl(pImpl) { }

RoleElement::RoleElement(const RoleElement& other)
    : m_pImpl(*other.m_pImpl) { }

Roles RoleElement::evaluate(const State& state) const {
    return m_pImpl->get()->evaluate(*state.m_pImpl);
}

unsigned RoleElement::complexity() const {
    return m_pImpl->get()->complexity();
}

std::string RoleElement::repr() const {
    return m_pImpl->get()->repr();
}


NumericalElement::NumericalElement(element::NumericalElement_Ptr pImpl)
    : m_pImpl(pImpl) { }

NumericalElement::NumericalElement(const NumericalElement& other)
    : m_pImpl(*other.m_pImpl) { }

int NumericalElement::evaluate(const State& state) const {
    return m_pImpl->get()->evaluate(*(state.m_pImpl));
}

unsigned NumericalElement::complexity() const {
    return m_pImpl->get()->complexity();
}

std::string NumericalElement::repr() const {
    return m_pImpl->get()->repr();
}


BooleanElement::BooleanElement(element::BooleanElement_Ptr pImpl)
    : m_pImpl(pImpl) { }

BooleanElement::BooleanElement(const BooleanElement& other)
    : m_pImpl(*other.m_pImpl) { }

bool BooleanElement::evaluate(const State& state) const {
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

ConceptElement ElementFactory::parse_concept_element(const std::string &description) {
    return ConceptElement(m_pImpl->parse_concept_element(description));
}

RoleElement ElementFactory::parse_role_element(const std::string &description) {
    return RoleElement(m_pImpl->parse_role_element(description));
}

NumericalElement ElementFactory::parse_numerical_element(const std::string &description) {
    return NumericalElement(m_pImpl->parse_numerical_element(description));
}

BooleanElement ElementFactory::parse_boolean_element(const std::string &description) {
    return BooleanElement(m_pImpl->parse_boolean_element(description));
}

NumericalElement ElementFactory::make_count_element(const ConceptElement& element) {
    return NumericalElement(m_pImpl->make_count_element(*element.m_pImpl));
}

NumericalElement ElementFactory::make_count_element(const RoleElement& element) {
    return NumericalElement(m_pImpl->make_count_element(*element.m_pImpl));
}

ConceptElement ElementFactory::make_primitive_concept_element(const std::string& name, unsigned pos) {
    return ConceptElement(m_pImpl->make_primitive_concept_element(name, pos));
}

ConceptElement ElementFactory::make_and_concept_element(const ConceptElement& element1, const ConceptElement& element2) {
    return ConceptElement(m_pImpl->make_and_concept_element(*element1.m_pImpl, *element2.m_pImpl));
}

}
}
