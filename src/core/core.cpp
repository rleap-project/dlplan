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
    if (!std::all_of(atoms.begin(), atoms.end(), [&](const Atom& atom){ return atom.get_instance_info() == &*m_pImpl; })) {
        throw std::runtime_error("InstanceInfo::convert_state - atom does not belong to the same instance.");
    }
    return State(m_pImpl->convert_state(atoms));
}

State InstanceInfo::convert_state(const Index_Vec& atom_idxs) const {
    return State(m_pImpl->convert_state(atom_idxs));
}

Atom InstanceInfo::add_atom(const std::string &predicate_name, const Name_Vec &object_names) {
    return Atom(m_pImpl->add_atom(predicate_name, object_names));
}

Atom InstanceInfo::add_static_atom(const std::string &predicate_name, const Name_Vec &object_names) {
    return Atom(m_pImpl->add_static_atom(predicate_name, object_names));
}

std::vector<Atom> InstanceInfo::get_atoms() const {
    std::vector<Atom> atoms;
    atoms.reserve(m_pImpl->get_atoms().size());
    for (AtomImpl atom : m_pImpl->get_atoms()) {
        atoms.push_back(Atom(std::move(atom)));
    }
    return atoms;
}

Atom InstanceInfo::get_atom(unsigned atom_idx) const {
    AtomImpl atom = m_pImpl->get_atom(atom_idx);
    return Atom(std::move(atom));
}

std::vector<Object> InstanceInfo::get_objects() const {
    std::vector<Object> objects;
    objects.reserve(m_pImpl->get_objects().size());
    for (ObjectImpl object : m_pImpl->get_objects()) {
        objects.push_back(Object(std::move(object)));
    }
    return objects;
}

Object InstanceInfo::get_object(unsigned object_idx) const {
    ObjectImpl object = m_pImpl->get_object(object_idx);
    return Object(std::move(object));
}

unsigned InstanceInfo::get_object_idx(const std::string& object_name) const {
    return m_pImpl->get_object_idx(object_name);
}

unsigned InstanceInfo::get_num_objects() const {
    return m_pImpl->get_num_objects();
}

const std::shared_ptr<const VocabularyInfoImpl>& InstanceInfo::get_vocabulary_info() const {
    return m_pImpl->get_vocabulary_info();
}


VocabularyInfo::VocabularyInfo() : m_pImpl(VocabularyInfoImpl()) { }

VocabularyInfo::~VocabularyInfo() { }

Predicate VocabularyInfo::add_predicate(const std::string &predicate_name, unsigned arity) {
    return Predicate(m_pImpl->add_predicate(predicate_name, arity));
}

InstanceInfo VocabularyInfo::make_instance() {
    return InstanceInfo(std::move(m_pImpl->make_instance()));
}

SyntacticElementFactory VocabularyInfo::make_factory() {
    return SyntacticElementFactoryImpl(std::move(m_pImpl->make_factory()));
}


Predicate::Predicate(PredicateImpl&& impl) : m_pImpl(impl) { }

Predicate::Predicate(const Predicate& other) : m_pImpl(*other.m_pImpl) { }

Predicate::~Predicate() {}

const VocabularyInfoImpl* Predicate::get_vocabulary_info() const {
    return m_pImpl->get_vocabulary_info();
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
    return m_pImpl->m_atom_name;
}

int Atom::get_atom_idx() const {
    return m_pImpl->m_atom_idx;
}

const std::string& Atom::get_predicate_name() const {
    return m_pImpl->get_predicate_name();
}

int Atom::get_predicate_idx() const {
    return m_pImpl->get_predicate_idx();
}

const Name_Vec& Atom::get_object_names() const {
    return m_pImpl->get_object_names();
}

const std::string& Atom::get_object_name(int pos) const {
    return m_pImpl->get_object_name(pos);
}

const Index_Vec& Atom::get_object_idxs() const {
    return m_pImpl->get_object_idxs();
}

int Atom::get_object_idx(int pos) const {
    return m_pImpl->get_object_idx(pos);
}

bool Atom::get_is_static() const {
    return m_pImpl->get_is_static();
}


State::State(StateImpl&& impl) : m_pImpl(std::move(impl)) { }

State::State(const State& other) : m_pImpl(*other.m_pImpl) {}

State::~State() { }

std::string State::str() const { /* tba */ }

std::shared_ptr<const InstanceInfoImpl> State::get_instance_info() const {
    return m_pImpl->get_instance_info();
}

const Index_Vec& State::get_atom_idxs() const {
    return m_pImpl->get_atom_idxs();
}


Concept::Concept(const VocabularyInfoImpl& parent, element::Concept_Ptr pImpl)
    : Element<ConceptDenotation>(parent), m_pImpl(pImpl) { }

Concept::Concept(const Concept& other)
    : Element<ConceptDenotation>(other), m_pImpl(*other.m_pImpl) { }

ConceptDenotation Concept::evaluate(const State& state) const {
    if (state.get_instance_info()->get_vocabulary_info().get() != m_parent) {
        throw std::runtime_error("Concept::evaluate - mismatched vocabularies of Concept and State.");
    }
    return m_pImpl->get()->evaluate(*state.m_pImpl);
}

unsigned Concept::compute_complexity() const {
    return m_pImpl->get()->compute_complexity();
}

std::string Concept::compute_repr() const {
    return m_pImpl->get()->compute_repr();
}

Role::Role(const VocabularyInfoImpl& parent, element::Role_Ptr pImpl)
    : Element<RoleDenotation>(parent), m_pImpl(pImpl) { }

Role::Role(const Role& other)
    : Element<RoleDenotation>(other), m_pImpl(*other.m_pImpl) { }

RoleDenotation Role::evaluate(const State& state) const {
    if (state.get_instance_info()->get_vocabulary_info().get() != m_parent) {
        throw std::runtime_error("Role::evaluate - mismatched vocabularies of Role and State.");
    }
    return m_pImpl->get()->evaluate(*state.m_pImpl);
}

unsigned Role::compute_complexity() const {
    return m_pImpl->get()->compute_complexity();
}

std::string Role::compute_repr() const {
    return m_pImpl->get()->compute_repr();
}


Numerical::Numerical(const VocabularyInfoImpl& parent, element::Numerical_Ptr pImpl)
    : Element<int>(parent), m_pImpl(pImpl) { }

Numerical::Numerical(const Numerical& other)
    : Element<int>(other), m_pImpl(*other.m_pImpl) { }

int Numerical::evaluate(const State& state) const {
    if (state.get_instance_info()->get_vocabulary_info().get() != m_parent) {
        throw std::runtime_error("Numerical::evaluate - mismatched vocabularies of Numerical and State.");
    }
    return m_pImpl->get()->evaluate(*state.m_pImpl);
}

unsigned Numerical::compute_complexity() const {
    return m_pImpl->get()->compute_complexity();
}

std::string Numerical::compute_repr() const {
    return m_pImpl->get()->compute_repr();
}


Boolean::Boolean(const VocabularyInfoImpl& parent, element::Boolean_Ptr pImpl)
    : Element<bool>(parent), m_pImpl(pImpl) { }

Boolean::Boolean(const Boolean& other)
    : Element<bool>(other), m_pImpl(*other.m_pImpl) { }

bool Boolean::evaluate(const State& state) const {
    if (state.get_instance_info()->get_vocabulary_info().get() != m_parent) {
        throw std::runtime_error("Boolean::evaluate - mismatched vocabularies of Concept and State.");
    }
    return m_pImpl->get()->evaluate(*state.m_pImpl);
}

unsigned Boolean::compute_complexity() const {
    return m_pImpl->get()->compute_complexity();
}

std::string Boolean::compute_repr() const {
    return m_pImpl->get()->compute_repr();
}


SyntacticElementFactory::SyntacticElementFactory(SyntacticElementFactoryImpl&& impl) : m_pImpl(std::move(impl)) { }

SyntacticElementFactory::SyntacticElementFactory(const SyntacticElementFactory& other) : m_pImpl(*other.m_pImpl) { }

SyntacticElementFactory::~SyntacticElementFactory() { }

Concept SyntacticElementFactory::parse_concept(const std::string &description) {
    element::Concept_Ptr concept = m_pImpl->parse_concept(description);
    if (!concept) throw std::runtime_error("SyntacticElementFactory::parse_concept - ("s + description + ") cannot be parsed into a Concept.");
    return Concept(*m_pImpl->get_vocabulary_info().get(), concept);
}

Role SyntacticElementFactory::parse_role(const std::string &description) {
    element::Role_Ptr role = m_pImpl->parse_role(description);
    if (!role) throw std::runtime_error("SyntacticElementFactory::parse_role - ("s + description + ") cannot be parsed into a Role.");
    return Role(*m_pImpl->get_vocabulary_info().get(), role);
}

Numerical SyntacticElementFactory::parse_numerical(const std::string &description) {
    element::Numerical_Ptr numerical = m_pImpl->parse_numerical(description);
    if (!numerical) throw std::runtime_error("SyntacticElementFactory::parse_numerical - ("s + description + ") cannot be parsed into a Numerical.");
    return Numerical(*m_pImpl->get_vocabulary_info().get(), numerical);
}

Boolean SyntacticElementFactory::parse_boolean(const std::string &description) {
    element::Boolean_Ptr boolean = m_pImpl->parse_boolean(description);
    if (!boolean) throw std::runtime_error("SyntacticElementFactory::parse_boolean - ("s + description + ") cannot be parsed into a Boolean.");
    return Boolean(*m_pImpl->get_vocabulary_info(), boolean);
}

Boolean SyntacticElementFactory::make_empty_boolean(const Concept& concept) {
    return Boolean(*m_pImpl->get_vocabulary_info(), m_pImpl->make_empty_boolean(*concept.m_pImpl));
}

Boolean SyntacticElementFactory::make_empty_boolean(const Role& role) {
    return Boolean(*m_pImpl->get_vocabulary_info(), m_pImpl->make_empty_boolean(*role.m_pImpl));
}

Concept SyntacticElementFactory::make_all_concept(const Role& role, const Concept& concept) {
    return Concept(*m_pImpl->get_vocabulary_info(), m_pImpl->make_all_concept(*role.m_pImpl, *concept.m_pImpl));
}

Concept SyntacticElementFactory::make_and_concept(const Concept& concept_left, const Concept& concept_right) {
    return Concept(*m_pImpl->get_vocabulary_info(), m_pImpl->make_and_concept(*concept_left.m_pImpl, *concept_right.m_pImpl));
}

Concept SyntacticElementFactory::make_bot_concept() {
    return Concept(*m_pImpl->get_vocabulary_info(), m_pImpl->make_bot_concept());
}

Concept SyntacticElementFactory::make_diff_concept(const Concept& concept_left, const Concept& concept_right) {
    return Concept(*m_pImpl->get_vocabulary_info(), m_pImpl->make_diff_concept(*concept_left.m_pImpl, *concept_right.m_pImpl));
}

Concept SyntacticElementFactory::make_not_concept(const Concept& concept) {
    return Concept(*m_pImpl->get_vocabulary_info(), m_pImpl->make_not_concept(*concept.m_pImpl));
}

Concept SyntacticElementFactory::make_one_of_concept(const std::string& object_name) {
    return Concept(*m_pImpl->get_vocabulary_info(), m_pImpl->make_one_of_concept(object_name));
}

Concept SyntacticElementFactory::make_or_concept(const Concept& concept_left, const Concept& concept_right) {
    return Concept(*m_pImpl->get_vocabulary_info(), m_pImpl->make_or_concept(*concept_left.m_pImpl, *concept_right.m_pImpl));
}

Concept SyntacticElementFactory::make_primitive_concept(const std::string& name, unsigned pos) {
    return Concept(*m_pImpl->get_vocabulary_info(), m_pImpl->make_primitive_concept(name, pos));
}

Concept SyntacticElementFactory::make_some_concept(const Role& role, const Concept& concept) {
    return Concept(*m_pImpl->get_vocabulary_info(), m_pImpl->make_some_concept(*role.m_pImpl, *concept.m_pImpl));
}

Concept SyntacticElementFactory::make_subset_concept(const Role& role_left, const Role& role_right) {
    return Concept(*m_pImpl->get_vocabulary_info(), m_pImpl->make_subset_concept(*role_left.m_pImpl, *role_right.m_pImpl));
}

Concept SyntacticElementFactory::make_top_concept() {
    return Concept(*m_pImpl->get_vocabulary_info(), m_pImpl->make_top_concept());
}

Numerical SyntacticElementFactory::make_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to) {
    return Numerical(*m_pImpl->get_vocabulary_info(), m_pImpl->make_concept_distance(*concept_from.m_pImpl, *role.m_pImpl, *concept_to.m_pImpl));
}

Numerical SyntacticElementFactory::make_count(const Concept& element) {
    return Numerical(*m_pImpl->get_vocabulary_info(), m_pImpl->make_count(*element.m_pImpl));
}

Numerical SyntacticElementFactory::make_count(const Role& element) {
    return Numerical(*m_pImpl->get_vocabulary_info(), m_pImpl->make_count(*element.m_pImpl));
}

Numerical SyntacticElementFactory::make_role_distance(const Role& role_from, const Role& role, const Role& role_to) {
    return Numerical(*m_pImpl->get_vocabulary_info(), m_pImpl->make_role_distance(*role_from.m_pImpl, *role.m_pImpl, *role_to.m_pImpl));
}

Numerical SyntacticElementFactory::make_sum_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to) {
    return Numerical(*m_pImpl->get_vocabulary_info(), m_pImpl->make_sum_concept_distance(*concept_from.m_pImpl, *role.m_pImpl, *concept_to.m_pImpl));
}

Numerical SyntacticElementFactory::make_sum_role_distance(const Role& role_from, const Role& role, const Role& role_to) {
    return Numerical(*m_pImpl->get_vocabulary_info(), m_pImpl->make_sum_role_distance(*role_from.m_pImpl, *role.m_pImpl, *role_to.m_pImpl));
}

Role SyntacticElementFactory::make_and_role(const Role& role_left, const Role& role_right) {
    return Role(*m_pImpl->get_vocabulary_info(), m_pImpl->make_and_role(*role_left.m_pImpl, *role_right.m_pImpl));
}

Role SyntacticElementFactory::make_compose_role(const Role& role_left, const Role& role_right) {
    return Role(*m_pImpl->get_vocabulary_info(), m_pImpl->make_compose_role(*role_left.m_pImpl, *role_right.m_pImpl));
}

Role SyntacticElementFactory::make_diff_role(const Role& role_left, const Role& role_right) {
    return Role(*m_pImpl->get_vocabulary_info(), m_pImpl->make_diff_role(*role_left.m_pImpl, *role_right.m_pImpl));
}

Role SyntacticElementFactory::make_identity_role(const Concept& concept) {
    return Role(*m_pImpl->get_vocabulary_info(), m_pImpl->make_identity_role(*concept.m_pImpl));
}

Role SyntacticElementFactory::make_inverse_role(const Role& role) {
    return Role(*m_pImpl->get_vocabulary_info(), m_pImpl->make_inverse_role(*role.m_pImpl));
}

Role SyntacticElementFactory::make_not_role(const Role& role) {
    return Role(*m_pImpl->get_vocabulary_info(), m_pImpl->make_not_role(*role.m_pImpl));
}

Role SyntacticElementFactory::make_or_role(const Role& role_left, const Role& role_right) {
    return Role(*m_pImpl->get_vocabulary_info(), m_pImpl->make_or_role(*role_left.m_pImpl, *role_right.m_pImpl));
}

Role SyntacticElementFactory::make_primitive_role(const std::string& name, unsigned pos_1, unsigned pos_2) {
    return Role(*m_pImpl->get_vocabulary_info(), m_pImpl->make_primitive_role(name, pos_1, pos_2));
}

Role SyntacticElementFactory::make_restrict_role(const Role& role, const Concept& concept) {
    return Role(*m_pImpl->get_vocabulary_info(), m_pImpl->make_restrict_role(*role.m_pImpl, *concept.m_pImpl));
}

Role SyntacticElementFactory::make_top_role() {
    return Role(*m_pImpl->get_vocabulary_info(), m_pImpl->make_top_role());
}

Role SyntacticElementFactory::make_transitive_closure(const Role& role) {
    return Role(*m_pImpl->get_vocabulary_info(), m_pImpl->make_transitive_closure(*role.m_pImpl));
}

Role SyntacticElementFactory::make_transitive_reflexive_closure(const Role& role) {
    return Role(*m_pImpl->get_vocabulary_info(), m_pImpl->make_transitive_reflexive_closure(*role.m_pImpl));
}

}
}
