#include "dlp/core.h"

#include "elements/concept.h"
#include "elements/role.h"
#include "elements/numerical.h"
#include "elements/boolean.h"

#include "element_factory.h"
#include "instance_info.h"
#include "vocabulary_info.h"
#include "atom.h"
#include "object.h"
#include "state.h"
#include "predicate.h"
#include "concept.h"
#include "role.h"
#include "numerical.h"
#include "boolean.h"

#include <iostream>
#include <algorithm>


namespace dlp {
namespace core {

InstanceInfo::InstanceInfo(InstanceInfoImpl&& impl) : m_pImpl(std::move(impl)) { }

InstanceInfo::~InstanceInfo() { }

State InstanceInfo::parse_state(const Name_Vec& atom_names) const {
    return State(m_pImpl->parse_state(atom_names));
}

State InstanceInfo::convert_state(const std::vector<Atom>& atoms) const {
    return State(m_pImpl->convert_state(atoms));
}

State InstanceInfo::convert_state(const Index_Vec& atom_idxs) const {
    return State(m_pImpl->convert_state(atom_idxs));
}

const Atom& InstanceInfo::add_atom(const std::string &predicate_name, const Name_Vec &object_names) {
    return m_pImpl->add_atom(predicate_name, object_names);
}

const Atom& InstanceInfo::add_static_atom(const std::string &predicate_name, const Name_Vec &object_names) {
    return m_pImpl->add_static_atom(predicate_name, object_names);
}

const VocabularyInfoImpl* InstanceInfo::get_vocabulary_info() const {
    return m_pImpl->get_vocabulary_info();
}


VocabularyInfo::VocabularyInfo() : m_pImpl(VocabularyInfoImpl()) { }

VocabularyInfo::~VocabularyInfo() { }

const Predicate& VocabularyInfo::add_predicate(const std::string &predicate_name, unsigned arity) {
    return m_pImpl->add_predicate(predicate_name, arity);
}

InstanceInfo VocabularyInfo::make_instance() {
    return m_pImpl->make_instance();
}

SyntacticElementFactory VocabularyInfo::make_factory() {
    return m_pImpl->make_factory();
}


Predicate::Predicate(PredicateImpl&& impl) : m_pImpl(impl) { }

Predicate::Predicate(const Predicate& other) : m_pImpl(*other.m_pImpl) { }

Predicate::~Predicate() {}

const VocabularyInfoImpl* Predicate::get_vocabulary_info() const {
    return m_pImpl->get_vocabulary_info();
}

int Predicate::get_predicate_idx() const {
    return m_pImpl->get_predicate_idx();
}

const std::string& Predicate::get_name() const {
    return m_pImpl->get_predicate_name();
}

unsigned Predicate::get_arity() const {
    return m_pImpl->get_arity();
}


Object::Object(ObjectImpl&& impl) : m_pImpl(std::move(impl)) {}

Object::Object(const Object& other) : m_pImpl(*other.m_pImpl) { }

Object::~Object() { }

const InstanceInfoImpl* Object::get_instance_info() const {
    return m_pImpl->get_instance_info();
}

const std::string& Object::get_object_name() const {
    return m_pImpl->get_object_name();
}

int Object::get_object_idx() const {
    return m_pImpl->get_object_idx();
}


Atom::Atom(AtomImpl&& impl) : m_pImpl(std::move(impl)) { }

Atom::Atom(const Atom& other) : m_pImpl(*other.m_pImpl) { }

Atom::~Atom() { }

const InstanceInfoImpl* Atom::get_instance_info() const {
    return m_pImpl->get_instance_info();
}

const std::string& Atom::get_atom_name() const {
    return m_pImpl->get_atom_name();
}

int Atom::get_atom_idx() const {
    return m_pImpl->get_atom_idx();
}

const Predicate& Atom::get_predicate() const {
    return m_pImpl->get_predicate();
}

const std::vector<Object>& Atom::get_objects() const {
    return m_pImpl->get_objects();
}

const Object& Atom::get_object(int pos) const {
    return m_pImpl->get_object(pos);
}

bool Atom::get_is_static() const {
    return m_pImpl->get_is_static();
}


State::State(StateImpl&& impl) : m_pImpl(std::move(impl)) { }

State::State(const State& other) : m_pImpl(*other.m_pImpl) {}

State::~State() { }

std::string State::str() const { /* tba */ }

const InstanceInfoImpl* State::get_instance_info() const {
    return m_pImpl->get_instance_info();
}

const Index_Vec& State::get_atom_idxs() const {
    return m_pImpl->get_atom_idxs();
}


Concept::Concept(ConceptImpl&& impl) : m_pImpl(std::move(impl)) { }

Concept::Concept(const Concept& other)
    : Element<ConceptDenotation>(other), m_pImpl(*other.m_pImpl) { }

Concept::~Concept() { }

ConceptDenotation Concept::evaluate(const State& state) {
    return m_pImpl->evaluate(state);
}

unsigned Concept::compute_complexity() const {
    return m_pImpl->compute_complexity();
}

std::string Concept::compute_repr() const {
    return m_pImpl->compute_repr();
}

Role::Role(RoleImpl&& impl) : m_pImpl(std::move(impl)) { }

Role::Role(const Role& other)
    : m_pImpl(*other.m_pImpl) { }

Role::~Role() { }

RoleDenotation Role::evaluate(const State& state) {
    return m_pImpl->evaluate(state);
}

unsigned Role::compute_complexity() const {
    return m_pImpl->compute_complexity();
}

std::string Role::compute_repr() const {
    return m_pImpl->compute_repr();
}


Numerical::Numerical(NumericalImpl&& impl) : m_pImpl(std::move(impl)) { }

Numerical::Numerical(const Numerical& other)
    : m_pImpl(*other.m_pImpl) { }

Numerical::~Numerical() { }

int Numerical::evaluate(const State& state) {
    return m_pImpl->evaluate(state);
}

unsigned Numerical::compute_complexity() const {
    return m_pImpl->compute_complexity();
}

std::string Numerical::compute_repr() const {
    return m_pImpl->compute_repr();
}


Boolean::Boolean(BooleanImpl&& impl) : m_pImpl(std::move(impl)) { }

Boolean::Boolean(const Boolean& other)
    : m_pImpl(*other.m_pImpl) { }

Boolean::~Boolean() { }

bool Boolean::evaluate(const State& state) {
    return m_pImpl->evaluate(state);
}

unsigned Boolean::compute_complexity() const {
    return m_pImpl->compute_complexity();
}

std::string Boolean::compute_repr() const {
    return m_pImpl->compute_repr();
}


SyntacticElementFactory::SyntacticElementFactory(SyntacticElementFactoryImpl&& impl) : m_pImpl(std::move(impl)) { }

SyntacticElementFactory::SyntacticElementFactory(const SyntacticElementFactory& other) : m_pImpl(*other.m_pImpl) { }

SyntacticElementFactory::~SyntacticElementFactory() { }

Concept SyntacticElementFactory::parse_concept(const std::string &description) {
    return m_pImpl->parse_concept(description);
}

Role SyntacticElementFactory::parse_role(const std::string &description) {
    return m_pImpl->parse_role(description);
}

Numerical SyntacticElementFactory::parse_numerical(const std::string &description) {
    return m_pImpl->parse_numerical(description);
}

Boolean SyntacticElementFactory::parse_boolean(const std::string &description) {
    return m_pImpl->parse_boolean(description);
}


Boolean SyntacticElementFactory::make_empty_boolean(const Concept& concept) {
    return m_pImpl->make_empty_boolean(concept);
}

Boolean SyntacticElementFactory::make_empty_boolean(const Role& role) {
    return m_pImpl->make_empty_boolean(role);
}

Concept SyntacticElementFactory::make_all_concept(const Role& role, const Concept& concept) {
    return m_pImpl->make_all_concept(role, concept);
}

Concept SyntacticElementFactory::make_and_concept(const Concept& concept_left, const Concept& concept_right) {
    return m_pImpl->make_and_concept(concept_left, concept_right);
}

Concept SyntacticElementFactory::make_bot_concept() {
    return m_pImpl->make_bot_concept();
}

Concept SyntacticElementFactory::make_diff_concept(const Concept& concept_left, const Concept& concept_right) {
    return m_pImpl->make_diff_concept(concept_left, concept_right);
}

Concept SyntacticElementFactory::make_not_concept(const Concept& concept) {
    return m_pImpl->make_not_concept(concept);
}

Concept SyntacticElementFactory::make_one_of_concept(const std::string& object_name) {
    return m_pImpl->make_one_of_concept(object_name);
}

Concept SyntacticElementFactory::make_or_concept(const Concept& concept_left, const Concept& concept_right) {
    return m_pImpl->make_or_concept(concept_left, concept_right);
}

Concept SyntacticElementFactory::make_primitive_concept(const std::string& name, unsigned pos) {
    return m_pImpl->make_primitive_concept(name, pos);
}

Concept SyntacticElementFactory::make_some_concept(const Role& role, const Concept& concept) {
    return m_pImpl->make_some_concept(role, concept);
}

Concept SyntacticElementFactory::make_subset_concept(const Role& role_left, const Role& role_right) {
    return m_pImpl->make_subset_concept(role_left, role_right);
}

Concept SyntacticElementFactory::make_top_concept() {
    return m_pImpl->make_top_concept();
}

Numerical SyntacticElementFactory::make_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to) {
    return m_pImpl->make_concept_distance(concept_from, role, concept_to);
}

Numerical SyntacticElementFactory::make_count(const Concept& concept) {
    return m_pImpl->make_count(concept);
}

Numerical SyntacticElementFactory::make_count(const Role& role) {
    return m_pImpl->make_count(role);
}

Numerical SyntacticElementFactory::make_role_distance(const Role& role_from, const Role& role, const Role& role_to) {
    return m_pImpl->make_role_distance(role_from, role, role_to);
}

Numerical SyntacticElementFactory::make_sum_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to) {
    return m_pImpl->make_sum_concept_distance(concept_from, role, concept_to);
}

Numerical SyntacticElementFactory::make_sum_role_distance(const Role& role_from, const Role& role, const Role& role_to) {
    return m_pImpl->make_sum_role_distance(role_from, role, role_to);
}

Role SyntacticElementFactory::make_and_role(const Role& role_left, const Role& role_right) {
    return m_pImpl->make_and_role(role_left, role_right);
}

Role SyntacticElementFactory::make_compose_role(const Role& role_left, const Role& role_right) {
    return m_pImpl->make_compose_role(role_left, role_right);
}

Role SyntacticElementFactory::make_diff_role(const Role& role_left, const Role& role_right) {
    return m_pImpl->make_diff_role(role_left, role_right);
}

Role SyntacticElementFactory::make_identity_role(const Concept& concept) {
    return m_pImpl->make_identity_role(concept);
}

Role SyntacticElementFactory::make_inverse_role(const Role& role) {
    return m_pImpl->make_inverse_role(role);
}

Role SyntacticElementFactory::make_not_role(const Role& role) {
    return m_pImpl->make_not_role(role);
}

Role SyntacticElementFactory::make_or_role(const Role& role_left, const Role& role_right) {
    return m_pImpl->make_or_role(role_left, role_right);
}

Role SyntacticElementFactory::make_primitive_role(const std::string& name, unsigned pos_1, unsigned pos_2) {
    return m_pImpl->make_primitive_role(name, pos_1, pos_2);
}

Role SyntacticElementFactory::make_restrict_role(const Role& role, const Concept& concept) {
    return m_pImpl->make_restrict_role(role, concept);
}

Role SyntacticElementFactory::make_top_role() {
    return m_pImpl->make_top_role();
}

Role SyntacticElementFactory::make_transitive_closure(const Role& role) {
    return m_pImpl->make_transitive_closure(role);
}

Role SyntacticElementFactory::make_transitive_reflexive_closure(const Role& role) {
    return m_pImpl->make_transitive_reflexive_closure(role);
}

}
}
