#include "dlp/core.h"

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
            throw std::runtime_error("InstanceInfo::parse_state - atom ("s + atom.get_atom_name() + ") does not belong to the instance");
        }
        atoms_impl.push_back(*atom.m_pImpl);
    }
    return State(*m_pImpl, m_pImpl->get()->convert_state(*m_pImpl, atoms_impl));
}

State InstanceInfo::convert_state(const Index_Vec& atom_idxs) const {
    return State(*m_pImpl, m_pImpl->get()->convert_state(*m_pImpl, atom_idxs));
}

Atom InstanceInfo::add_atom(const std::string &predicate_name, const Name_Vec &object_names) {
    return Atom(*m_pImpl, m_pImpl->get()->add_atom(predicate_name, object_names));
}

Atom InstanceInfo::add_static_atom(const std::string &predicate_name, const Name_Vec &object_names) {
    return Atom(*m_pImpl, m_pImpl->get()->add_static_atom(predicate_name, object_names));
}

std::vector<Predicate> InstanceInfo::get_predicates() const {
    std::vector<Predicate> predicates;
    for (PredicateImpl predicate_impl : m_pImpl->get()->get_predicates()) {
        predicates.push_back(Predicate(*m_pImpl, std::move(predicate_impl)));
    }
    return predicates;
}


Predicate::Predicate(std::shared_ptr<InstanceInfoImpl> parent, PredicateImpl&& impl) : m_parent(parent), m_pImpl(impl) { }

Predicate::Predicate(const Predicate& other) : m_pImpl(*other.m_pImpl) { }

Predicate::~Predicate() {}

const std::string& Predicate::get_name() const {
    return m_pImpl->m_predicate_name;
}

unsigned Predicate::get_arity() const {
    return m_pImpl->m_arity;
}


Atom::Atom(std::shared_ptr<InstanceInfoImpl> parent, AtomImpl&& impl) : m_parent(parent), m_pImpl(std::move(impl)) { }

Atom::Atom(const Atom& other) : m_parent(other.m_parent), m_pImpl(*other.m_pImpl) { }

Atom::~Atom() { }

int Atom::get_atom_idx() const {
    return m_pImpl->m_atom_idx;
}

const std::string& Atom::get_atom_name() const {
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

unsigned ConceptElement::compute_complexity() const {
    return m_pImpl->get()->compute_complexity();
}

std::string ConceptElement::compute_repr() const {
    return m_pImpl->get()->compute_repr();
}

RoleElement::RoleElement(element::RoleElement_Ptr pImpl)
    : m_pImpl(pImpl) { }

RoleElement::RoleElement(const RoleElement& other)
    : m_pImpl(*other.m_pImpl) { }

Roles RoleElement::evaluate(const State& state) const {
    return m_pImpl->get()->evaluate(*state.m_pImpl);
}

unsigned RoleElement::compute_complexity() const {
    return m_pImpl->get()->compute_complexity();
}

std::string RoleElement::compute_repr() const {
    return m_pImpl->get()->compute_repr();
}


NumericalElement::NumericalElement(element::NumericalElement_Ptr pImpl)
    : m_pImpl(pImpl) { }

NumericalElement::NumericalElement(const NumericalElement& other)
    : m_pImpl(*other.m_pImpl) { }

int NumericalElement::evaluate(const State& state) const {
    return m_pImpl->get()->evaluate(*(state.m_pImpl));
}

unsigned NumericalElement::compute_complexity() const {
    return m_pImpl->get()->compute_complexity();
}

std::string NumericalElement::compute_repr() const {
    return m_pImpl->get()->compute_repr();
}


BooleanElement::BooleanElement(element::BooleanElement_Ptr pImpl)
    : m_pImpl(pImpl) { }

BooleanElement::BooleanElement(const BooleanElement& other)
    : m_pImpl(*other.m_pImpl) { }

bool BooleanElement::evaluate(const State& state) const {
    return m_pImpl->get()->evaluate(*state.m_pImpl);
}

unsigned BooleanElement::compute_complexity() const {
    return m_pImpl->get()->compute_complexity();
}

std::string BooleanElement::compute_repr() const {
    return m_pImpl->get()->compute_repr();
}


SyntacticElementFactory::SyntacticElementFactory() { }

SyntacticElementFactory::~SyntacticElementFactory() { }

ConceptElement SyntacticElementFactory::parse_concept_element(const std::string &description) {
    return ConceptElement(m_pImpl->parse_concept_element(description));
}

RoleElement SyntacticElementFactory::parse_role_element(const std::string &description) {
    return RoleElement(m_pImpl->parse_role_element(description));
}

NumericalElement SyntacticElementFactory::parse_numerical_element(const std::string &description) {
    return NumericalElement(m_pImpl->parse_numerical_element(description));
}

BooleanElement SyntacticElementFactory::parse_boolean_element(const std::string &description) {
    return BooleanElement(m_pImpl->parse_boolean_element(description));
}

BooleanElement SyntacticElementFactory::make_empty_boolean_element(const ConceptElement& concept) {
    return BooleanElement(m_pImpl->make_empty_boolean_element(*concept.m_pImpl));
}

BooleanElement SyntacticElementFactory::make_empty_boolean_element(const RoleElement& role) {
    return BooleanElement(m_pImpl->make_empty_boolean_element(*role.m_pImpl));
}

ConceptElement SyntacticElementFactory::make_all_concept_element(const RoleElement& role, const ConceptElement& concept) {
    return ConceptElement(m_pImpl->make_all_concept_element(*role.m_pImpl, *concept.m_pImpl));
}

ConceptElement SyntacticElementFactory::make_and_concept_element(const ConceptElement& concept_left, const ConceptElement& concept_right) {
    return ConceptElement(m_pImpl->make_and_concept_element(*concept_left.m_pImpl, *concept_right.m_pImpl));
}

ConceptElement SyntacticElementFactory::make_bot_concept_element() {
    return ConceptElement(m_pImpl->make_bot_concept_element());
}

ConceptElement SyntacticElementFactory::make_diff_concept_element(const ConceptElement& concept_left, const ConceptElement& concept_right) {
    return ConceptElement(m_pImpl->make_diff_concept_element(*concept_left.m_pImpl, *concept_right.m_pImpl));
}

ConceptElement SyntacticElementFactory::make_not_concept_element(const ConceptElement& concept) {
    return ConceptElement(m_pImpl->make_not_concept_element(*concept.m_pImpl));
}

ConceptElement SyntacticElementFactory::make_one_of_concept_element(unsigned object_idx) {
    return ConceptElement(m_pImpl->make_one_of_concept_element(object_idx));
}

ConceptElement SyntacticElementFactory::make_or_concept_element(const ConceptElement& concept_left, const ConceptElement& concept_right) {
    return ConceptElement(m_pImpl->make_or_concept_element(*concept_left.m_pImpl, *concept_right.m_pImpl));
}

ConceptElement SyntacticElementFactory::make_primitive_concept_element(const std::string& name, unsigned pos) {
    return ConceptElement(m_pImpl->make_primitive_concept_element(name, pos));
}

ConceptElement SyntacticElementFactory::make_some_concept_element(const RoleElement& role, const ConceptElement& concept) {
    return ConceptElement(m_pImpl->make_some_concept_element(*role.m_pImpl, *concept.m_pImpl));
}

ConceptElement SyntacticElementFactory::make_subset_concept_element(const RoleElement& role_left, const RoleElement& role_right) {
    return ConceptElement(m_pImpl->make_subset_concept_element(*role_left.m_pImpl, *role_right.m_pImpl));
}

ConceptElement SyntacticElementFactory::make_top_concept_element() {
    return ConceptElement(m_pImpl->make_top_concept_element());
}

NumericalElement SyntacticElementFactory::make_concept_distance_element(const ConceptElement& concept_from, const RoleElement& role, const ConceptElement& concept_to) {
    return NumericalElement(m_pImpl->make_concept_distance_element(*concept_from.m_pImpl, *role.m_pImpl, *concept_to.m_pImpl));
}

NumericalElement SyntacticElementFactory::make_count_element(const ConceptElement& element) {
    return NumericalElement(m_pImpl->make_count_element(*element.m_pImpl));
}

NumericalElement SyntacticElementFactory::make_count_element(const RoleElement& element) {
    return NumericalElement(m_pImpl->make_count_element(*element.m_pImpl));
}

NumericalElement SyntacticElementFactory::make_role_distance_element(const RoleElement& role_from, const RoleElement& role, const RoleElement& role_to) {
    return NumericalElement(m_pImpl->make_role_distance_element(*role_from.m_pImpl, *role.m_pImpl, *role_to.m_pImpl));
}

NumericalElement SyntacticElementFactory::make_sum_concept_distance_element(const ConceptElement& concept_from, const RoleElement& role, const ConceptElement& concept_to) {
    return NumericalElement(m_pImpl->make_sum_concept_distance_element(*concept_from.m_pImpl, *role.m_pImpl, *concept_to.m_pImpl));
}

NumericalElement SyntacticElementFactory::make_sum_role_distance_element(const RoleElement& role_from, const RoleElement& role, const RoleElement& role_to) {
    return NumericalElement(m_pImpl->make_sum_role_distance_element(*role_from.m_pImpl, *role.m_pImpl, *role_to.m_pImpl));
}

RoleElement SyntacticElementFactory::make_and_role_element(const RoleElement& role_left, const RoleElement& role_right) {
    return RoleElement(m_pImpl->make_and_role_element(*role_left.m_pImpl, *role_right.m_pImpl));
}

RoleElement SyntacticElementFactory::make_compose_role_element(const RoleElement& role_left, const RoleElement& role_right) {
    return RoleElement(m_pImpl->make_compose_role_element(*role_left.m_pImpl, *role_right.m_pImpl));
}

RoleElement SyntacticElementFactory::make_diff_role_element(const RoleElement& role_left, const RoleElement& role_right) {
    return RoleElement(m_pImpl->make_diff_role_element(*role_left.m_pImpl, *role_right.m_pImpl));
}

RoleElement SyntacticElementFactory::make_identity_role_element(const ConceptElement& concept) {
    return RoleElement(m_pImpl->make_identity_role_element(*concept.m_pImpl));
}

RoleElement SyntacticElementFactory::make_inverse_role_element(const RoleElement& role) {
    return RoleElement(m_pImpl->make_inverse_role_element(*role.m_pImpl));
}

RoleElement SyntacticElementFactory::make_not_role_element(const RoleElement& role) {
    return RoleElement(m_pImpl->make_not_role_element(*role.m_pImpl));
}

RoleElement SyntacticElementFactory::make_or_role_element(const RoleElement& role_left, const RoleElement& role_right) {
    return RoleElement(m_pImpl->make_or_role_element(*role_left.m_pImpl, *role_right.m_pImpl));
}

RoleElement SyntacticElementFactory::make_primitive_role_element(const std::string& name, unsigned pos_1, unsigned pos_2) {
    return RoleElement(m_pImpl->make_primitive_role_element(name, pos_1, pos_2));
}

RoleElement SyntacticElementFactory::make_restrict_role_element(const RoleElement& role, const ConceptElement& concept) {
    return RoleElement(m_pImpl->make_restrict_role_element(*role.m_pImpl, *concept.m_pImpl));
}

RoleElement SyntacticElementFactory::make_top_role_element() {
    return RoleElement(m_pImpl->make_top_role_element());
}

RoleElement SyntacticElementFactory::make_transitive_closure_element(const RoleElement& role) {
    return RoleElement(m_pImpl->make_transitive_closure_element(*role.m_pImpl));
}

RoleElement SyntacticElementFactory::make_transitive_reflexive_closure_element(const RoleElement& role) {
    return RoleElement(m_pImpl->make_transitive_reflexive_closure_element(*role.m_pImpl));
}

}
}
