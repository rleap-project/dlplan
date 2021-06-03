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

InstanceInfo::InstanceInfo(const VocabularyInfo& vocabulary_info)
    : m_vocabulary_info(*vocabulary_info.m_pImpl), m_pImpl(std::make_shared<InstanceInfoImpl>(InstanceInfoImpl(**vocabulary_info.m_pImpl))) { }

// InstanceInfo::InstanceInfo(const InstanceInfo& other) : m_parent(other.m_parent), m_pImpl(other.m_pImpl) { }

InstanceInfo::~InstanceInfo() { }

State InstanceInfo::parse_state(const Name_Vec& atom_names) const {
    return State(**m_pImpl, m_pImpl->get()->parse_state(atom_names));
}

State InstanceInfo::convert_state(const std::vector<Atom>& atoms) const {
    std::vector<AtomImpl> atoms_impl;
    atoms_impl.reserve(atoms.size());
    for (const auto& atom : atoms) {
        if (atom.m_parent != m_pImpl->get()) {
            throw std::runtime_error("InstanceInfo::convert_state - atom ("s + atom.get_atom_name() + ") does not belong to the same instance.");
        }
        atoms_impl.push_back(*atom.m_pImpl);
    }
    return State(**m_pImpl, m_pImpl->get()->convert_state(atoms_impl));
}

State InstanceInfo::convert_state(const Index_Vec& atom_idxs) const {
    return State(**m_pImpl, m_pImpl->get()->convert_state(atom_idxs));
}

Atom InstanceInfo::add_atom(const std::string &predicate_name, const Name_Vec &object_names) {
    return Atom(**m_pImpl, m_pImpl->get()->add_atom(predicate_name, object_names));
}

Atom InstanceInfo::add_static_atom(const std::string &predicate_name, const Name_Vec &object_names) {
    return Atom(**m_pImpl, m_pImpl->get()->add_static_atom(predicate_name, object_names));
}


VocabularyInfo::VocabularyInfo() : m_pImpl(std::make_shared<VocabularyInfoImpl>(VocabularyInfoImpl())) { }

VocabularyInfo::VocabularyInfo(const VocabularyInfo& other) : m_pImpl(*other.m_pImpl) { }

VocabularyInfo::~VocabularyInfo() { }

Predicate VocabularyInfo::add_predicate(const std::string &predicate_name, unsigned arity) {
    return Predicate(**m_pImpl, m_pImpl->get()->add_predicate(predicate_name, arity));
}


Predicate::Predicate(const VocabularyInfoImpl& parent, PredicateImpl&& impl) : m_parent(&parent), m_pImpl(impl) { }

Predicate::Predicate(const Predicate& other) : m_pImpl(*other.m_pImpl) { }

Predicate::~Predicate() {}

const std::string& Predicate::get_name() const {
    return m_pImpl->m_predicate_name;
}

unsigned Predicate::get_arity() const {
    return m_pImpl->m_arity;
}


Atom::Atom(const InstanceInfoImpl& parent, AtomImpl&& impl) : m_parent(&parent), m_pImpl(std::move(impl)) { }

Atom::Atom(const Atom& other) : m_parent(other.m_parent), m_pImpl(*other.m_pImpl) { }

Atom::~Atom() { }

int Atom::get_atom_idx() const {
    return m_pImpl->m_atom_idx;
}

const std::string& Atom::get_atom_name() const {
    return m_pImpl->m_atom_name;
}


State::State(const InstanceInfoImpl& instance_info, StateImpl&& impl) : m_instance_info(instance_info.shared_from_this()), m_pImpl(std::move(impl)) { }

State::State(const State& other) : m_pImpl(*other.m_pImpl) {}

State::~State() { }

std::string State::str() const { /* tba */ }


Concept::Concept(const VocabularyInfoImpl& parent, element::Concept_Ptr pImpl)
    : Element<ConceptDenotation>(parent), m_pImpl(pImpl) { }

Concept::Concept(const Concept& other)
    : Element<ConceptDenotation>(other), m_pImpl(*other.m_pImpl) { }

ConceptDenotation Concept::evaluate(const State& state) const {
    if (state.m_pImpl->get_instance_info()->get_vocabulary_info().get() != m_parent) {
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
    if (state.m_pImpl->get_instance_info()->get_vocabulary_info().get() != m_parent) {
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
    if (state.m_pImpl->get_instance_info()->get_vocabulary_info().get() != m_parent) {
        throw std::runtime_error("Numerical::evaluate - mismatched vocabularies of Numerical and State.");
    }
    return m_pImpl->get()->evaluate(*(state.m_pImpl));
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
    if (state.m_pImpl->get_instance_info()->get_vocabulary_info().get() != m_parent) {
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


SyntacticElementFactory::SyntacticElementFactory(const VocabularyInfo& vocabulary_info)
    : m_vocabulary_info(vocabulary_info.m_pImpl->get()->shared_from_this()), m_pImpl(SyntacticElementFactoryImpl(**vocabulary_info.m_pImpl)) { }

SyntacticElementFactory::~SyntacticElementFactory() { }

Concept SyntacticElementFactory::parse_concept(const std::string &description) {
    element::Concept_Ptr concept = m_pImpl->parse_concept(description);
    if (!concept) throw std::runtime_error("SyntacticElementFactory::parse_concept - ("s + description + ") cannot be parsed into a Concept.");
    return Concept(*m_vocabulary_info, concept);
}

Role SyntacticElementFactory::parse_role(const std::string &description) {
    element::Role_Ptr role = m_pImpl->parse_role(description);
    if (!role) throw std::runtime_error("SyntacticElementFactory::parse_role - ("s + description + ") cannot be parsed into a Role.");
    return Role(*m_vocabulary_info, role);
}

Numerical SyntacticElementFactory::parse_numerical(const std::string &description) {
    element::Numerical_Ptr numerical = m_pImpl->parse_numerical(description);
    if (!numerical) throw std::runtime_error("SyntacticElementFactory::parse_numerical - ("s + description + ") cannot be parsed into a Numerical.");
    return Numerical(*m_vocabulary_info, numerical);
}

Boolean SyntacticElementFactory::parse_boolean(const std::string &description) {
    element::Boolean_Ptr boolean = m_pImpl->parse_boolean(description);
    if (!boolean) throw std::runtime_error("SyntacticElementFactory::parse_boolean - ("s + description + ") cannot be parsed into a Boolean.");
    return Boolean(*m_vocabulary_info, boolean);
}

Boolean SyntacticElementFactory::make_empty_boolean(const Concept& concept) {
    return Boolean(*m_vocabulary_info, m_pImpl->make_empty_boolean(*concept.m_pImpl));
}

Boolean SyntacticElementFactory::make_empty_boolean(const Role& role) {
    return Boolean(*m_vocabulary_info, m_pImpl->make_empty_boolean(*role.m_pImpl));
}

Concept SyntacticElementFactory::make_all_concept(const Role& role, const Concept& concept) {
    return Concept(*m_vocabulary_info, m_pImpl->make_all_concept(*role.m_pImpl, *concept.m_pImpl));
}

Concept SyntacticElementFactory::make_and_concept(const Concept& concept_left, const Concept& concept_right) {
    return Concept(*m_vocabulary_info, m_pImpl->make_and_concept(*concept_left.m_pImpl, *concept_right.m_pImpl));
}

Concept SyntacticElementFactory::make_bot_concept() {
    return Concept(*m_vocabulary_info, m_pImpl->make_bot_concept());
}

Concept SyntacticElementFactory::make_diff_concept(const Concept& concept_left, const Concept& concept_right) {
    return Concept(*m_vocabulary_info, m_pImpl->make_diff_concept(*concept_left.m_pImpl, *concept_right.m_pImpl));
}

Concept SyntacticElementFactory::make_not_concept(const Concept& concept) {
    return Concept(*m_vocabulary_info, m_pImpl->make_not_concept(*concept.m_pImpl));
}

Concept SyntacticElementFactory::make_one_of_concept(const std::string& object_name) {
    return Concept(*m_vocabulary_info, m_pImpl->make_one_of_concept(object_name));
}

Concept SyntacticElementFactory::make_or_concept(const Concept& concept_left, const Concept& concept_right) {
    return Concept(*m_vocabulary_info, m_pImpl->make_or_concept(*concept_left.m_pImpl, *concept_right.m_pImpl));
}

Concept SyntacticElementFactory::make_primitive_concept(const std::string& name, unsigned pos) {
    return Concept(*m_vocabulary_info, m_pImpl->make_primitive_concept(name, pos));
}

Concept SyntacticElementFactory::make_some_concept(const Role& role, const Concept& concept) {
    return Concept(*m_vocabulary_info, m_pImpl->make_some_concept(*role.m_pImpl, *concept.m_pImpl));
}

Concept SyntacticElementFactory::make_subset_concept(const Role& role_left, const Role& role_right) {
    return Concept(*m_vocabulary_info, m_pImpl->make_subset_concept(*role_left.m_pImpl, *role_right.m_pImpl));
}

Concept SyntacticElementFactory::make_top_concept() {
    return Concept(*m_vocabulary_info, m_pImpl->make_top_concept());
}

Numerical SyntacticElementFactory::make_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to) {
    return Numerical(*m_vocabulary_info, m_pImpl->make_concept_distance(*concept_from.m_pImpl, *role.m_pImpl, *concept_to.m_pImpl));
}

Numerical SyntacticElementFactory::make_count(const Concept& element) {
    return Numerical(*m_vocabulary_info, m_pImpl->make_count(*element.m_pImpl));
}

Numerical SyntacticElementFactory::make_count(const Role& element) {
    return Numerical(*m_vocabulary_info, m_pImpl->make_count(*element.m_pImpl));
}

Numerical SyntacticElementFactory::make_role_distance(const Role& role_from, const Role& role, const Role& role_to) {
    return Numerical(*m_vocabulary_info, m_pImpl->make_role_distance(*role_from.m_pImpl, *role.m_pImpl, *role_to.m_pImpl));
}

Numerical SyntacticElementFactory::make_sum_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to) {
    return Numerical(*m_vocabulary_info, m_pImpl->make_sum_concept_distance(*concept_from.m_pImpl, *role.m_pImpl, *concept_to.m_pImpl));
}

Numerical SyntacticElementFactory::make_sum_role_distance(const Role& role_from, const Role& role, const Role& role_to) {
    return Numerical(*m_vocabulary_info, m_pImpl->make_sum_role_distance(*role_from.m_pImpl, *role.m_pImpl, *role_to.m_pImpl));
}

Role SyntacticElementFactory::make_and_role(const Role& role_left, const Role& role_right) {
    return Role(*m_vocabulary_info, m_pImpl->make_and_role(*role_left.m_pImpl, *role_right.m_pImpl));
}

Role SyntacticElementFactory::make_compose_role(const Role& role_left, const Role& role_right) {
    return Role(*m_vocabulary_info, m_pImpl->make_compose_role(*role_left.m_pImpl, *role_right.m_pImpl));
}

Role SyntacticElementFactory::make_diff_role(const Role& role_left, const Role& role_right) {
    return Role(*m_vocabulary_info, m_pImpl->make_diff_role(*role_left.m_pImpl, *role_right.m_pImpl));
}

Role SyntacticElementFactory::make_identity_role(const Concept& concept) {
    return Role(*m_vocabulary_info, m_pImpl->make_identity_role(*concept.m_pImpl));
}

Role SyntacticElementFactory::make_inverse_role(const Role& role) {
    return Role(*m_vocabulary_info, m_pImpl->make_inverse_role(*role.m_pImpl));
}

Role SyntacticElementFactory::make_not_role(const Role& role) {
    return Role(*m_vocabulary_info, m_pImpl->make_not_role(*role.m_pImpl));
}

Role SyntacticElementFactory::make_or_role(const Role& role_left, const Role& role_right) {
    return Role(*m_vocabulary_info, m_pImpl->make_or_role(*role_left.m_pImpl, *role_right.m_pImpl));
}

Role SyntacticElementFactory::make_primitive_role(const std::string& name, unsigned pos_1, unsigned pos_2) {
    return Role(*m_vocabulary_info, m_pImpl->make_primitive_role(name, pos_1, pos_2));
}

Role SyntacticElementFactory::make_restrict_role(const Role& role, const Concept& concept) {
    return Role(*m_vocabulary_info, m_pImpl->make_restrict_role(*role.m_pImpl, *concept.m_pImpl));
}

Role SyntacticElementFactory::make_top_role() {
    return Role(*m_vocabulary_info, m_pImpl->make_top_role());
}

Role SyntacticElementFactory::make_transitive_closure(const Role& role) {
    return Role(*m_vocabulary_info, m_pImpl->make_transitive_closure(*role.m_pImpl));
}

Role SyntacticElementFactory::make_transitive_reflexive_closure(const Role& role) {
    return Role(*m_vocabulary_info, m_pImpl->make_transitive_reflexive_closure(*role.m_pImpl));
}

}
}
