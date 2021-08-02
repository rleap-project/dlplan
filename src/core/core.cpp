#include "dlplan/core.h"

#include "elements/concept.h"
#include "elements/role.h"
#include "elements/numerical.h"
#include "elements/boolean.h"

#include "element_factory.h"
#include "instance_info.h"
#include "vocabulary_info.h"
#include "constant.h"
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


namespace dlplan::core {

InstanceInfo::InstanceInfo(std::shared_ptr<const VocabularyInfo> vocabulary_info) : m_pImpl(InstanceInfoImpl(*this, vocabulary_info)) { }

InstanceInfo::~InstanceInfo() = default;

const Atom& InstanceInfo::add_atom(const std::string &name, const Name_Vec &object_names) {
    return m_pImpl->add_atom(*this, name, object_names);
}

const Atom& InstanceInfo::add_static_atom(const std::string &name, const Name_Vec &object_names) {
    return m_pImpl->add_static_atom(*this, name, object_names);
}

const std::vector<Atom>& InstanceInfo::get_atoms() const {
    return m_pImpl->get_atoms();
}

const Atom& InstanceInfo::get_atom(int index) const {
    return m_pImpl->get_atom(index);
}

int InstanceInfo::get_atom_idx(const std::string& name) const {
    return m_pImpl->get_atom_idx(name);
}

bool InstanceInfo::exists_object(const std::string name) const {
    return m_pImpl->exists_object(name);
}

const std::vector<Object>& InstanceInfo::get_objects() const {
    return m_pImpl->get_objects();
}

const Object& InstanceInfo::get_object(int index) const {
    return m_pImpl->get_object(index);
}

int InstanceInfo::get_object_idx(const std::string& name) const {
    return m_pImpl->get_object_idx(name);
}

int InstanceInfo::get_num_objects() const {
    return m_pImpl->get_num_objects();
}

std::shared_ptr<const VocabularyInfo> InstanceInfo::get_vocabulary_info() const {
    return m_pImpl->get_vocabulary_info();
}

const Index_Vec& InstanceInfo::get_static_atom_idxs() const {
    return m_pImpl->get_static_atom_idxs();
}


VocabularyInfo::VocabularyInfo() : m_pImpl(VocabularyInfoImpl()) { }

VocabularyInfo::VocabularyInfo(const VocabularyInfo& other) : m_pImpl(*other.m_pImpl) { }

VocabularyInfo::~VocabularyInfo() = default;

const Predicate& VocabularyInfo::add_predicate(const std::string &name, int arity) {
    return m_pImpl->add_predicate(*this, name, arity);
}

const Constant& VocabularyInfo::add_constant(const std::string& name) {
    return m_pImpl->add_constant(*this, name);
}

bool VocabularyInfo::exists_predicate_name(const std::string& name) const {
    return m_pImpl->exists_predicate_name(name);
}

const std::vector<Predicate>& VocabularyInfo::get_predicates() const {
    return m_pImpl->get_predicates();
}

int VocabularyInfo::get_predicate_idx(const std::string& name) const {
    return m_pImpl->get_predicate_idx(name);
}

const Predicate& VocabularyInfo::get_predicate(int index) const {
    return m_pImpl->get_predicate(index);
}

bool VocabularyInfo::exists_constant_name(const std::string& name) const {
    return m_pImpl->exists_constant_name(name);
}

int VocabularyInfo::get_constant_idx(const std::string& name) const {
    return m_pImpl->get_constant_idx(name);
}

const Constant& VocabularyInfo::get_constant(int index) const {
    return m_pImpl->get_constant(index);
}

const std::vector<Constant>& VocabularyInfo::get_constants() const {
    return m_pImpl->get_constants();
}


Constant::Constant(const VocabularyInfo* vocabulary_info, const std::string& name, int index)
    : m_pImpl(ConstantImpl(vocabulary_info, name, index)) { }

Constant::Constant(const Constant& other) : m_pImpl(*other.m_pImpl) { }

Constant::~Constant() { }

bool Constant::operator==(const Constant& other) {
    return (get_index() == other.get_index() && (get_vocabulary_info() == other.get_vocabulary_info()));
}

bool Constant::operator!=(const Constant& other) {
    return !(*this == other);
}

const VocabularyInfo* Constant::get_vocabulary_info() const {
    return m_pImpl->get_vocabulary_info();
}

int Constant::get_index() const {
    return m_pImpl->get_index();
}

const std::string& Constant::get_name() const {
    return m_pImpl->get_name();
}


Predicate::Predicate(const VocabularyInfo* vocabulary_info, const std::string& name, int index, int arity)
    : m_pImpl(PredicateImpl(vocabulary_info, name, index, arity)) { }

Predicate::Predicate(const Predicate& other) : m_pImpl(*other.m_pImpl) { }

Predicate::~Predicate() = default;

bool Predicate::operator==(const Predicate& other)  {
    return (get_index() == other.get_index()) && (get_vocabulary_info() == other.get_vocabulary_info());
}

bool Predicate::operator!=(const Predicate& other) {
    return !(*this == other);
}

const VocabularyInfo* Predicate::get_vocabulary_info() const {
    return m_pImpl->get_vocabulary_info();
}

int Predicate::get_index() const {
    return m_pImpl->get_index();
}

const std::string& Predicate::get_name() const {
    return m_pImpl->get_name();
}

int Predicate::get_arity() const {
    return m_pImpl->get_arity();
}


Object::Object(const InstanceInfo* instance_info, const std::string& name, int index)
    : m_pImpl(ObjectImpl(instance_info, name, index)) {}

Object::Object(const Object& other) : m_pImpl(*other.m_pImpl) { }

Object::~Object() = default;

bool Object::operator==(const Object& other) {
    return (get_index() == other.get_index()) && (get_instance_info() == other.get_instance_info());
}

bool Object::operator!=(const Object& other) {
    return !(*this == other);
}

const InstanceInfo* Object::get_instance_info() const {
    return m_pImpl->get_instance_info();
}

const std::string& Object::get_name() const {
    return m_pImpl->get_name();
}

int Object::get_index() const {
    return m_pImpl->get_index();
}


Atom::Atom(const InstanceInfo* instance_info,
    const std::string& name,
    int index,
    const Predicate& predicate,
    const std::vector<Object> &objects,
    bool is_static) : m_pImpl(AtomImpl(instance_info, name, index, predicate, objects, is_static)) { }

Atom::Atom(const Atom& other) : m_pImpl(*other.m_pImpl) { }

Atom::~Atom() = default;

bool Atom::operator==(const Atom& other) {
    return (get_index() == other.get_index()) && (get_instance_info() == other.get_instance_info());
}

bool Atom::operator!=(const Atom& other) {
    return !(*this == other);
}

const InstanceInfo* Atom::get_instance_info() const {
    return m_pImpl->get_instance_info();
}

const std::string& Atom::get_name() const {
    return m_pImpl->get_name();
}

int Atom::get_index() const {
    return m_pImpl->get_index();
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


State::State(std::shared_ptr<const InstanceInfo> instance_info, const std::vector<Atom>& atoms) : m_pImpl(instance_info, atoms) { }

State::State(std::shared_ptr<const InstanceInfo> instance_info, const Name_Vec& atom_names) : m_pImpl(instance_info, atom_names) { }

State::State(std::shared_ptr<const InstanceInfo> instance_info, const Index_Vec& atom_indices) : m_pImpl(instance_info, atom_indices) { }

State::State(const State& other) : m_pImpl(*other.m_pImpl) {}

State::~State() = default;

bool State::operator==(const State& other) {
    return (get_atom_idxs() == other.get_atom_idxs()) && (get_instance_info() == other.get_instance_info());
}

bool State::operator!=(const State& other) {
    return !(*this == other);
}

std::string State::str() const { return m_pImpl->str(); }

std::shared_ptr<const InstanceInfo> State::get_instance_info() const {
    return m_pImpl->get_instance_info();
}

const Index_Vec& State::get_atom_idxs() const {
    return m_pImpl->get_atom_idxs();
}


Concept::Concept(const VocabularyInfo& vocabulary_info, element::Concept_Ptr&& concept)
    : m_pImpl(ConceptImpl(vocabulary_info, std::move(concept))) { }

Concept::Concept(const Concept& other)
    : m_pImpl(ConceptImpl(*other.m_pImpl)) { }

Concept::~Concept() = default;

ConceptDenotation Concept::evaluate(const State& state) const {
    return m_pImpl->evaluate(state);
}

int Concept::compute_complexity() const {
    return m_pImpl->compute_complexity();
}

std::string Concept::compute_repr() const {
    return m_pImpl->compute_repr();
}

Role::Role(const VocabularyInfo& vocabulary_info, element::Role_Ptr&& role)
    : m_pImpl(RoleImpl(vocabulary_info, std::move(role))) { }

Role::Role(const Role& other)
    : m_pImpl(RoleImpl(*other.m_pImpl)) { }

Role::~Role() = default;

RoleDenotation Role::evaluate(const State& state) const {
    return m_pImpl->evaluate(state);
}

int Role::compute_complexity() const {
    return m_pImpl->compute_complexity();
}

std::string Role::compute_repr() const {
    return m_pImpl->compute_repr();
}


Numerical::Numerical(const VocabularyInfo& vocabulary_info, element::Numerical_Ptr&& numerical)
    : m_pImpl(NumericalImpl(vocabulary_info, std::move(numerical))) { }

Numerical::Numerical(const Numerical& other)
    : m_pImpl(NumericalImpl(*other.m_pImpl)) { }

Numerical::~Numerical() = default;

int Numerical::evaluate(const State& state) const {
    return m_pImpl->evaluate(state);
}

int Numerical::compute_complexity() const {
    return m_pImpl->compute_complexity();
}

std::string Numerical::compute_repr() const {
    return m_pImpl->compute_repr();
}


Boolean::Boolean(const VocabularyInfo& vocabulary_info, element::Boolean_Ptr&& boolean)
    : m_pImpl(BooleanImpl(vocabulary_info, std::move(boolean))) { }

Boolean::Boolean(const Boolean& other)
    : m_pImpl(BooleanImpl(*other.m_pImpl)) { }

Boolean::~Boolean() = default;

bool Boolean::evaluate(const State& state) const {
    return m_pImpl->evaluate(state);
}

int Boolean::compute_complexity() const {
    return m_pImpl->compute_complexity();
}

std::string Boolean::compute_repr() const {
    return m_pImpl->compute_repr();
}


SyntacticElementFactory::SyntacticElementFactory(std::shared_ptr<const VocabularyInfo> vocabulary_info) : m_pImpl(SyntacticElementFactoryImpl(vocabulary_info)) { }

SyntacticElementFactory::SyntacticElementFactory(const SyntacticElementFactory& other) : m_pImpl(*other.m_pImpl) { }

SyntacticElementFactory::~SyntacticElementFactory() = default;

const VocabularyInfo* SyntacticElementFactory::get_vocabulary_info() const {
    return m_pImpl->get_vocabulary_info();
}

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

Concept SyntacticElementFactory::make_one_of_concept(const Constant& constant) {
    return m_pImpl->make_one_of_concept(constant);
}

Concept SyntacticElementFactory::make_or_concept(const Concept& concept_left, const Concept& concept_right) {
    return m_pImpl->make_or_concept(concept_left, concept_right);
}

Concept SyntacticElementFactory::make_projection_concept(const Role& role, int pos) {
    return m_pImpl->make_projection_concept(role, pos);
}

Concept SyntacticElementFactory::make_primitive_concept(const Predicate& predicate, int pos) {
    return m_pImpl->make_primitive_concept(predicate, pos);
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

Role SyntacticElementFactory::make_primitive_role(const Predicate& predicate, int pos_1, int pos_2) {
    return m_pImpl->make_primitive_role(predicate, pos_1, pos_2);
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
