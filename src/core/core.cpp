#include "../../include/dlplan/core.h"

#include <iostream>
#include <algorithm>

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
#include "elements/types.h"


namespace dlplan::core {

ConceptDenotation::ConceptDenotation(int num_objects)
    : m_num_objects(num_objects), m_data(dynamic_bitset::DynamicBitset<unsigned>(num_objects)) { }

ConceptDenotation::ConceptDenotation(int num_objects, dynamic_bitset::DynamicBitset<unsigned>&& data)
    : m_num_objects(num_objects), m_data(std::move(data)) { }

ConceptDenotation::~ConceptDenotation() { }

std::vector<int> ConceptDenotation::to_vector() const {
    std::vector<int> result;
    result.reserve(m_num_objects);
    for (int i = 0; i < m_num_objects; ++i) {
        if (m_data.test(i)) result.push_back(i);
    }
    result.shrink_to_fit();
    return result;
}

int ConceptDenotation::get_num_objects() const {
    return m_num_objects;
}

dynamic_bitset::DynamicBitset<unsigned>& ConceptDenotation::get_data() {
    return m_data;
}

const dynamic_bitset::DynamicBitset<unsigned>& ConceptDenotation::get_const_data() const {
    return m_data;
}


RoleDenotation::RoleDenotation(int num_objects)
    : m_num_objects(num_objects), m_data(dynamic_bitset::DynamicBitset<unsigned>(num_objects * num_objects)) { }

RoleDenotation::RoleDenotation(int num_objects, dynamic_bitset::DynamicBitset<unsigned>&& data)
    : m_num_objects(num_objects), m_data(std::move(data)) { }

RoleDenotation::~RoleDenotation() { }

std::vector<std::pair<int, int>> RoleDenotation::to_vector() const {
    std::vector<std::pair<int, int>> result;
    result.reserve(m_num_objects * m_num_objects);
    for (int i = 0; i < m_num_objects; ++i) {
        for (int j = 0; j < m_num_objects; ++j) {
            if (m_data.test(i * m_num_objects + j)) {
                result.emplace_back(i, j);
            }
        }
    }
    result.shrink_to_fit();
    return result;
}

int RoleDenotation::get_num_objects() const {
    return m_num_objects;
}

dynamic_bitset::DynamicBitset<unsigned>& RoleDenotation::get_data() {
    return m_data;
}

const dynamic_bitset::DynamicBitset<unsigned>& RoleDenotation::get_const_data() const {
    return m_data;
}


InstanceInfo::InstanceInfo(std::shared_ptr<const VocabularyInfo> vocabulary_info) : m_pImpl(InstanceInfoImpl(vocabulary_info)) { }

InstanceInfo& InstanceInfo::operator=(const InstanceInfo& other) {
    if (this != &other) {
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

InstanceInfo::~InstanceInfo() = default;

const Atom& InstanceInfo::add_atom(const std::string &name, const Name_Vec &object_names, bool negated) {
    return m_pImpl->add_atom(name, object_names, negated);
}

const Atom& InstanceInfo::add_static_atom(const std::string &name, const Name_Vec &object_names) {
    return m_pImpl->add_static_atom(name, object_names);
}

bool InstanceInfo::exists_atom(const Atom& atom) const {
    return m_pImpl->exists_atom(atom);
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

bool InstanceInfo::exists_object(const Object& object) const {
    return m_pImpl->exists_object(object);
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

const ConceptDenotation& InstanceInfo::get_top_concept() const {
    return m_pImpl->get_top_concept();
}

const RoleDenotation& InstanceInfo::get_top_role() const {
    return m_pImpl->get_top_role();
}

size_t InstanceInfo::compute_hash() const {
    return m_pImpl->compute_hash();
}



VocabularyInfo::VocabularyInfo() : m_pImpl(VocabularyInfoImpl()) { }

VocabularyInfo::VocabularyInfo(const VocabularyInfo& other) : m_pImpl(*other.m_pImpl) { }

VocabularyInfo& VocabularyInfo::operator=(const VocabularyInfo& other) {
    if (this != &other) {
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

VocabularyInfo::~VocabularyInfo() = default;

const Predicate& VocabularyInfo::add_predicate(const std::string &name, int arity) {
    return m_pImpl->add_predicate(name, arity);
}

const Constant& VocabularyInfo::add_constant(const std::string& name) {
    return m_pImpl->add_constant(name);
}

bool VocabularyInfo::exists_predicate(const Predicate& predicate) const {
    return m_pImpl->exists_predicate(predicate);
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

bool VocabularyInfo::exists_constant(const Constant& constant) const {
    return m_pImpl->exists_constant(constant);
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

size_t VocabularyInfo::compute_hash() const {
    return m_pImpl->compute_hash();
}


Constant::Constant(const std::string& name, int index)
    : m_pImpl(ConstantImpl(name, index)) { }

Constant::Constant(const Constant& other) : m_pImpl(*other.m_pImpl) { }

Constant& Constant::operator=(const Constant& other) {
    if (this != &other) {
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

Constant::~Constant() { }

bool Constant::operator==(const Constant& other) const {
    return (get_index() == other.get_index() && (get_name() == other.get_name()));
}

bool Constant::operator!=(const Constant& other) const {
    return !(*this == other);
}

int Constant::get_index() const {
    return m_pImpl->get_index();
}

const std::string& Constant::get_name() const {
    return m_pImpl->get_name();
}


Predicate::Predicate(const std::string& name, int index, int arity)
    : m_pImpl(PredicateImpl(name, index, arity)) { }

Predicate::Predicate(const Predicate& other) : m_pImpl(*other.m_pImpl) { }

Predicate& Predicate::operator=(const Predicate& other) {
    if (this != &other) {
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

Predicate::~Predicate() = default;

bool Predicate::operator==(const Predicate& other) const {
    return (get_index() == other.get_index()) && (get_name() == other.get_name() && get_arity() == other.get_arity());
}

bool Predicate::operator!=(const Predicate& other) const {
    return !(*this == other);
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


Object::Object(const std::string& name, int index)
    : m_pImpl(ObjectImpl(name, index)) {}

Object::Object(const Object& other) : m_pImpl(*other.m_pImpl) { }

Object& Object::operator=(const Object& other) {
    if (this != &other) {
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

Object::~Object() = default;

bool Object::operator==(const Object& other) const {
    return (get_index() == other.get_index()) && (get_name() == other.get_name());
}

bool Object::operator!=(const Object& other) const {
    return !(*this == other);
}

const std::string& Object::get_name() const {
    return m_pImpl->get_name();
}

int Object::get_index() const {
    return m_pImpl->get_index();
}


Atom::Atom(const std::string& name,
    int index,
    const Predicate& predicate,
    const std::vector<Object> &objects,
    bool is_static) : m_pImpl(AtomImpl(name, index, predicate, objects, is_static)) { }

Atom::Atom(const Atom& other) : m_pImpl(*other.m_pImpl) { }

Atom& Atom::operator=(const Atom& other) {
    if (this != &other) {
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

Atom::~Atom() = default;

bool Atom::operator==(const Atom& other) const {
    return (get_index() == other.get_index()) && (get_name() == other.get_name());
}

bool Atom::operator!=(const Atom& other) const {
    return !(*this == other);
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


State::State(std::shared_ptr<const InstanceInfo> instance_info, const std::vector<Atom>& atoms) : m_pImpl(StateImpl(instance_info, atoms)) { }

State::State(std::shared_ptr<const InstanceInfo> instance_info, const Index_Vec& atom_idxs) : m_pImpl(StateImpl(instance_info, atom_idxs)) { }

State::State(const State& other) : m_pImpl(*other.m_pImpl) {}

State& State::operator=(const State& other) {
    if (this != &other) {
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

State::~State() = default;

bool State::operator==(const State& other) const {
    return (get_atom_idxs() == other.get_atom_idxs()) && (get_instance_info() == other.get_instance_info());
}

bool State::operator!=(const State& other) const {
    return !(*this == other);
}

std::string State::str() const { return m_pImpl->str(); }

std::shared_ptr<const InstanceInfo> State::get_instance_info() const {
    return m_pImpl->get_instance_info();
}

const Index_Vec& State::get_atom_idxs() const {
    return m_pImpl->get_atom_idxs();
}

size_t State::compute_hash() const {
    return m_pImpl->compute_hash();
}


Concept::Concept(std::shared_ptr<const VocabularyInfo> vocabulary_info, element::Concept_Ptr&& concept)
    : Element<ConceptDenotation>(vocabulary_info, concept), m_pImpl(ConceptImpl(concept)) { }

Concept::Concept(const Concept& other)
    : Element<ConceptDenotation>(other.get_vocabulary_info(), other.get_element()), m_pImpl(ConceptImpl(*other.m_pImpl)) { }

Concept& Concept::operator=(const Concept& other) {
    if (this != &other) {
        Element<ConceptDenotation>::operator=(other);
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

Concept::~Concept() = default;

ConceptDenotation Concept::evaluate(const State& state) const {
    return m_pImpl->evaluate(this, state);
}

int Concept::compute_complexity() const {
    return m_element->compute_complexity();
}

std::string Concept::compute_repr() const {
    return m_element->compute_repr();
}

Role::Role(std::shared_ptr<const VocabularyInfo> vocabulary_info, element::Role_Ptr&& role)
    : Element<RoleDenotation>(vocabulary_info, role), m_pImpl(RoleImpl(role)) { }

Role::Role(const Role& other)
    : Element<RoleDenotation>(other.get_vocabulary_info(), other.get_element()), m_pImpl(RoleImpl(*other.m_pImpl)) { }

Role& Role::operator=(const Role& other) {
    if (this != &other) {
        Element<RoleDenotation>::operator=(other);
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

Role::~Role() = default;

RoleDenotation Role::evaluate(const State& state) const {
    return m_pImpl->evaluate(this, state);
}

int Role::compute_complexity() const {
    return m_element->compute_complexity();
}

std::string Role::compute_repr() const {
    return m_element->compute_repr();
}


Numerical::Numerical(std::shared_ptr<const VocabularyInfo> vocabulary_info, element::Numerical_Ptr&& numerical)
    : Element<int>(vocabulary_info, numerical), m_pImpl(NumericalImpl(numerical)) { }

Numerical::Numerical(const Numerical& other)
    : Element<int>(other.get_vocabulary_info(), other.get_element()), m_pImpl(NumericalImpl(*other.m_pImpl)) { }

Numerical& Numerical::operator=(const Numerical& other) {
    if (this != &other) {
        Element<int>::operator=(other);
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

Numerical::~Numerical() = default;

int Numerical::evaluate(const State& state) const {
    return m_pImpl->evaluate(this, state);
}

int Numerical::compute_complexity() const {
    return m_element->compute_complexity();
}

std::string Numerical::compute_repr() const {
    return m_element->compute_repr();
}


Boolean::Boolean(std::shared_ptr<const VocabularyInfo> vocabulary_info, element::Boolean_Ptr&& boolean)
    : Element<bool>(vocabulary_info, boolean), m_pImpl(BooleanImpl(boolean)) { }

Boolean::Boolean(const Boolean& other)
    : Element<bool>(other.get_vocabulary_info(), other.get_element()), m_pImpl(BooleanImpl(*other.m_pImpl)) { }

Boolean& Boolean::operator=(const Boolean& other) {
    if (this != &other) {
        Element<bool>::operator=(other);
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

Boolean::~Boolean() = default;

bool Boolean::evaluate(const State& state) const {
    return m_pImpl->evaluate(this, state);
}

int Boolean::compute_complexity() const {
    return m_element->compute_complexity();
}

std::string Boolean::compute_repr() const {
    return m_element->compute_repr();
}


SyntacticElementFactory::SyntacticElementFactory(std::shared_ptr<const VocabularyInfo> vocabulary_info) : m_pImpl(SyntacticElementFactoryImpl(vocabulary_info)) { }

SyntacticElementFactory::SyntacticElementFactory(const SyntacticElementFactory& other) : m_pImpl(*other.m_pImpl) { }

SyntacticElementFactory& SyntacticElementFactory::operator=(const SyntacticElementFactory& other) {
    if (this != &other) {
        m_pImpl = other.m_pImpl;
    }
    return *this;
}

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

Boolean SyntacticElementFactory::make_concept_inclusion_boolean(const Concept& concept_left, const Concept& concept_right) {
    return m_pImpl->make_concept_inclusion_boolean(concept_left, concept_right);
}

Boolean SyntacticElementFactory::make_role_inclusion_boolean(const Role& role_left, const Role& role_right) {
    return m_pImpl->make_role_inclusion_boolean(role_left, role_right);
}

Boolean SyntacticElementFactory::make_nullary_boolean(const Predicate& predicate) {
    return m_pImpl->make_nullary_boolean(predicate);
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

Concept SyntacticElementFactory::make_equal_concept(const Role& role_left, const Role& role_right) {
    return m_pImpl->make_equal_concept(role_left, role_right);
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
