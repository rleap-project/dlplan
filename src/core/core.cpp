#include "dlp/core.h"

#include "elements/concept.h"
#include "elements/role.h"
#include "elements/numerical.h"
#include "elements/boolean.h"

#include "element_factory.h"
#include "instance_info.h"
#include "vocabulary_info.h"
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


VocabularyInfo::VocabularyInfo() : m_pImpl(std::make_shared<VocabularyInfoImpl>(VocabularyInfoImpl())) { }

VocabularyInfo::VocabularyInfo(const VocabularyInfo& other) : m_pImpl(*other.m_pImpl) { }

VocabularyInfo::~VocabularyInfo() { }

Predicate VocabularyInfo::add_predicate(const std::string &predicate_name, unsigned arity) {
    return Predicate(*m_pImpl, m_pImpl->get()->add_predicate(predicate_name, arity));
}


Predicate::Predicate(std::shared_ptr<VocabularyInfoImpl> parent, PredicateImpl&& impl) : m_parent(parent), m_pImpl(impl) { }

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


Concept::Concept(element::Concept_Ptr pImpl)
    : m_pImpl(pImpl) { }

Concept::Concept(const Concept& other)
    : m_pImpl(*other.m_pImpl) { }

ConceptDenotation Concept::evaluate(const State& state) const {
    return m_pImpl->get()->evaluate(*state.m_pImpl);
}

unsigned Concept::compute_complexity() const {
    return m_pImpl->get()->compute_complexity();
}

std::string Concept::compute_repr() const {
    return m_pImpl->get()->compute_repr();
}

Role::Role(element::Role_Ptr pImpl)
    : m_pImpl(pImpl) { }

Role::Role(const Role& other)
    : m_pImpl(*other.m_pImpl) { }

RoleDenotation Role::evaluate(const State& state) const {
    return m_pImpl->get()->evaluate(*state.m_pImpl);
}

unsigned Role::compute_complexity() const {
    return m_pImpl->get()->compute_complexity();
}

std::string Role::compute_repr() const {
    return m_pImpl->get()->compute_repr();
}


Numerical::Numerical(element::Numerical_Ptr pImpl)
    : m_pImpl(pImpl) { }

Numerical::Numerical(const Numerical& other)
    : m_pImpl(*other.m_pImpl) { }

int Numerical::evaluate(const State& state) const {
    return m_pImpl->get()->evaluate(*(state.m_pImpl));
}

unsigned Numerical::compute_complexity() const {
    return m_pImpl->get()->compute_complexity();
}

std::string Numerical::compute_repr() const {
    return m_pImpl->get()->compute_repr();
}


Boolean::Boolean(element::Boolean_Ptr pImpl)
    : m_pImpl(pImpl) { }

Boolean::Boolean(const Boolean& other)
    : m_pImpl(*other.m_pImpl) { }

bool Boolean::evaluate(const State& state) const {
    return m_pImpl->get()->evaluate(*state.m_pImpl);
}

unsigned Boolean::compute_complexity() const {
    return m_pImpl->get()->compute_complexity();
}

std::string Boolean::compute_repr() const {
    return m_pImpl->get()->compute_repr();
}


SyntacticElementFactory::SyntacticElementFactory(const VocabularyInfo& vocabulary_info)
    : m_pImpl(SyntacticElementFactoryImpl(*vocabulary_info.m_pImpl)) { }

SyntacticElementFactory::~SyntacticElementFactory() { }

Concept SyntacticElementFactory::parse_concept_element(const std::string &description) {
    return Concept(m_pImpl->parse_concept_element(description));
}

Role SyntacticElementFactory::parse_role_element(const std::string &description) {
    return Role(m_pImpl->parse_role_element(description));
}

Numerical SyntacticElementFactory::parse_numerical_element(const std::string &description) {
    return Numerical(m_pImpl->parse_numerical_element(description));
}

Boolean SyntacticElementFactory::parse_boolean_element(const std::string &description) {
    return Boolean(m_pImpl->parse_boolean_element(description));
}

Boolean SyntacticElementFactory::make_empty_boolean_element(const Concept& concept) {
    return Boolean(m_pImpl->make_empty_boolean_element(*concept.m_pImpl));
}

Boolean SyntacticElementFactory::make_empty_boolean_element(const Role& role) {
    return Boolean(m_pImpl->make_empty_boolean_element(*role.m_pImpl));
}

Concept SyntacticElementFactory::make_all_concept_element(const Role& role, const Concept& concept) {
    return Concept(m_pImpl->make_all_concept_element(*role.m_pImpl, *concept.m_pImpl));
}

Concept SyntacticElementFactory::make_and_concept_element(const Concept& concept_left, const Concept& concept_right) {
    return Concept(m_pImpl->make_and_concept_element(*concept_left.m_pImpl, *concept_right.m_pImpl));
}

Concept SyntacticElementFactory::make_bot_concept_element() {
    return Concept(m_pImpl->make_bot_concept_element());
}

Concept SyntacticElementFactory::make_diff_concept_element(const Concept& concept_left, const Concept& concept_right) {
    return Concept(m_pImpl->make_diff_concept_element(*concept_left.m_pImpl, *concept_right.m_pImpl));
}

Concept SyntacticElementFactory::make_not_concept_element(const Concept& concept) {
    return Concept(m_pImpl->make_not_concept_element(*concept.m_pImpl));
}

Concept SyntacticElementFactory::make_one_of_concept_element(unsigned object_idx) {
    return Concept(m_pImpl->make_one_of_concept_element(object_idx));
}

Concept SyntacticElementFactory::make_or_concept_element(const Concept& concept_left, const Concept& concept_right) {
    return Concept(m_pImpl->make_or_concept_element(*concept_left.m_pImpl, *concept_right.m_pImpl));
}

Concept SyntacticElementFactory::make_primitive_concept_element(const std::string& name, unsigned pos) {
    return Concept(m_pImpl->make_primitive_concept_element(name, pos));
}

Concept SyntacticElementFactory::make_some_concept_element(const Role& role, const Concept& concept) {
    return Concept(m_pImpl->make_some_concept_element(*role.m_pImpl, *concept.m_pImpl));
}

Concept SyntacticElementFactory::make_subset_concept_element(const Role& role_left, const Role& role_right) {
    return Concept(m_pImpl->make_subset_concept_element(*role_left.m_pImpl, *role_right.m_pImpl));
}

Concept SyntacticElementFactory::make_top_concept_element() {
    return Concept(m_pImpl->make_top_concept_element());
}

Numerical SyntacticElementFactory::make_concept_distance_element(const Concept& concept_from, const Role& role, const Concept& concept_to) {
    return Numerical(m_pImpl->make_concept_distance_element(*concept_from.m_pImpl, *role.m_pImpl, *concept_to.m_pImpl));
}

Numerical SyntacticElementFactory::make_count_element(const Concept& element) {
    return Numerical(m_pImpl->make_count_element(*element.m_pImpl));
}

Numerical SyntacticElementFactory::make_count_element(const Role& element) {
    return Numerical(m_pImpl->make_count_element(*element.m_pImpl));
}

Numerical SyntacticElementFactory::make_role_distance_element(const Role& role_from, const Role& role, const Role& role_to) {
    return Numerical(m_pImpl->make_role_distance_element(*role_from.m_pImpl, *role.m_pImpl, *role_to.m_pImpl));
}

Numerical SyntacticElementFactory::make_sum_concept_distance_element(const Concept& concept_from, const Role& role, const Concept& concept_to) {
    return Numerical(m_pImpl->make_sum_concept_distance_element(*concept_from.m_pImpl, *role.m_pImpl, *concept_to.m_pImpl));
}

Numerical SyntacticElementFactory::make_sum_role_distance_element(const Role& role_from, const Role& role, const Role& role_to) {
    return Numerical(m_pImpl->make_sum_role_distance_element(*role_from.m_pImpl, *role.m_pImpl, *role_to.m_pImpl));
}

Role SyntacticElementFactory::make_and_role_element(const Role& role_left, const Role& role_right) {
    return Role(m_pImpl->make_and_role_element(*role_left.m_pImpl, *role_right.m_pImpl));
}

Role SyntacticElementFactory::make_compose_role_element(const Role& role_left, const Role& role_right) {
    return Role(m_pImpl->make_compose_role_element(*role_left.m_pImpl, *role_right.m_pImpl));
}

Role SyntacticElementFactory::make_diff_role_element(const Role& role_left, const Role& role_right) {
    return Role(m_pImpl->make_diff_role_element(*role_left.m_pImpl, *role_right.m_pImpl));
}

Role SyntacticElementFactory::make_identity_role_element(const Concept& concept) {
    return Role(m_pImpl->make_identity_role_element(*concept.m_pImpl));
}

Role SyntacticElementFactory::make_inverse_role_element(const Role& role) {
    return Role(m_pImpl->make_inverse_role_element(*role.m_pImpl));
}

Role SyntacticElementFactory::make_not_role_element(const Role& role) {
    return Role(m_pImpl->make_not_role_element(*role.m_pImpl));
}

Role SyntacticElementFactory::make_or_role_element(const Role& role_left, const Role& role_right) {
    return Role(m_pImpl->make_or_role_element(*role_left.m_pImpl, *role_right.m_pImpl));
}

Role SyntacticElementFactory::make_primitive_role_element(const std::string& name, unsigned pos_1, unsigned pos_2) {
    return Role(m_pImpl->make_primitive_role_element(name, pos_1, pos_2));
}

Role SyntacticElementFactory::make_restrict_role_element(const Role& role, const Concept& concept) {
    return Role(m_pImpl->make_restrict_role_element(*role.m_pImpl, *concept.m_pImpl));
}

Role SyntacticElementFactory::make_top_role_element() {
    return Role(m_pImpl->make_top_role_element());
}

Role SyntacticElementFactory::make_transitive_closure_element(const Role& role) {
    return Role(m_pImpl->make_transitive_closure_element(*role.m_pImpl));
}

Role SyntacticElementFactory::make_transitive_reflexive_closure_element(const Role& role) {
    return Role(m_pImpl->make_transitive_reflexive_closure_element(*role.m_pImpl));
}

}
}
