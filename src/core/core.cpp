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
#include "elements/types.h"


namespace dlplan::core {

ConceptDenotationFlatSet::ConceptDenotationFlatSet(int num_objects) : m_num_objects(num_objects) { }

ConceptDenotationFlatSet::ConceptDenotationFlatSet(const ConceptDenotationFlatSet& other) = default;

ConceptDenotationFlatSet& ConceptDenotationFlatSet::operator=(const ConceptDenotationFlatSet& other) = default;

ConceptDenotationFlatSet::ConceptDenotationFlatSet(ConceptDenotationFlatSet&& other) = default;

ConceptDenotationFlatSet& ConceptDenotationFlatSet::operator=(ConceptDenotationFlatSet&& other) = default;

ConceptDenotationFlatSet::~ConceptDenotationFlatSet() = default;

ConceptDenotationFlatSet& ConceptDenotationFlatSet::operator&=(const ConceptDenotationFlatSet& other) {
    for (auto it = m_data.begin(); it != m_data.end();) {
        if (!other.contains(*it)) {
            it = m_data.erase(it);
        } else {
            ++it;
        }
    }
    return *this;
}

ConceptDenotationFlatSet& ConceptDenotationFlatSet::operator|=(const ConceptDenotationFlatSet& other) {
    for (const auto single : other.m_data) {
        m_data.insert(single);
    }
    return *this;
}

ConceptDenotationFlatSet& ConceptDenotationFlatSet::operator-=(const ConceptDenotationFlatSet& other) {
    for (const auto single : other.m_data) {
        m_data.erase(single);
    }
    return *this;
}

ConceptDenotationFlatSet& ConceptDenotationFlatSet::operator~() {
    phmap::flat_hash_set<int> result;
    for (int i = 0; i < m_num_objects; ++i) {
        if (m_data.count(i) == 0) {
            result.insert(i);
        }
    }
    m_data = result;
    return *this;
}

phmap::flat_hash_set<int>::const_iterator ConceptDenotationFlatSet::begin() const {
    return m_data.begin();
}

phmap::flat_hash_set<int>::const_iterator ConceptDenotationFlatSet::end() const {
    return m_data.end();
}

bool ConceptDenotationFlatSet::contains(int value) const {
    return static_cast<bool>(m_data.count(value));
}

void ConceptDenotationFlatSet::insert(int value) {
    m_data.insert(value);
}

void ConceptDenotationFlatSet::erase(int value) {
    m_data.erase(value);
}

int ConceptDenotationFlatSet::size() const {
    return m_data.size();
}

bool ConceptDenotationFlatSet::empty() const {
    return m_data.empty();
}

bool ConceptDenotationFlatSet::intersects(const ConceptDenotationFlatSet& other) const {
    for (const auto single : m_data) {
        if (other.contains(single)) return true;
    }
    return false;
}

bool ConceptDenotationFlatSet::is_subset_of(const ConceptDenotationFlatSet& other) const {
    for (const auto single : m_data) {
        if (!other.contains(single)) return false;
    }
    return true;
}

std::vector<int> ConceptDenotationFlatSet::to_sorted_vector() const {
    std::vector<int> result(m_data.begin(), m_data.end());
    std::sort(result.begin(), result.end());
    return result;
}

int ConceptDenotationFlatSet::get_num_objects() const {
    return m_num_objects;
}


RoleDenotationFlatSet::RoleDenotationFlatSet(int num_objects) : m_num_objects(num_objects) { }

RoleDenotationFlatSet::RoleDenotationFlatSet(const RoleDenotationFlatSet& other) = default;

RoleDenotationFlatSet& RoleDenotationFlatSet::operator=(const RoleDenotationFlatSet& other) = default;

RoleDenotationFlatSet::RoleDenotationFlatSet(RoleDenotationFlatSet&& other) = default;

RoleDenotationFlatSet& RoleDenotationFlatSet::operator=(RoleDenotationFlatSet&& other) = default;

RoleDenotationFlatSet::~RoleDenotationFlatSet() = default;

RoleDenotationFlatSet& RoleDenotationFlatSet::operator&=(const RoleDenotationFlatSet& other) {
    for (auto it = m_data.begin(); it != m_data.end();) {
        if (!other.contains(*it)) {
            it = m_data.erase(it);
        } else {
            ++it;
        }
    }
    return *this;
}

RoleDenotationFlatSet& RoleDenotationFlatSet::operator|=(const RoleDenotationFlatSet& other) {
    for (const auto single : other.m_data) {
        m_data.insert(single);
    }
    return *this;
}

RoleDenotationFlatSet& RoleDenotationFlatSet::operator-=(const RoleDenotationFlatSet& other) {
    for (const auto single : other.m_data) {
        m_data.erase(single);
    }
    return *this;
}

RoleDenotationFlatSet& RoleDenotationFlatSet::operator~() {
    phmap::flat_hash_set<std::pair<int, int>> result;
    for (int i = 0; i < m_num_objects; ++i) {
        for (int j = 0; j < m_num_objects; ++j) {
            std::pair<int, int> pair(i, j);
            if (m_data.count(pair) == 0) {
                result.insert(pair);
            }
        }
    }
    m_data = result;
    return *this;
}

phmap::flat_hash_set<std::pair<int, int>>::const_iterator RoleDenotationFlatSet::begin() const {
    return m_data.begin();
}

phmap::flat_hash_set<std::pair<int, int>>::const_iterator RoleDenotationFlatSet::end() const {
    return m_data.end();
}

bool RoleDenotationFlatSet::contains(const std::pair<int, int>& value) const {
    return static_cast<bool>(m_data.count(value));
}

void RoleDenotationFlatSet::insert(const std::pair<int, int>& value) {
    m_data.insert(value);
}

void RoleDenotationFlatSet::erase(const std::pair<int, int>& value) {
    m_data.erase(value);
}

int RoleDenotationFlatSet::size() const {
    return m_data.size();
}

bool RoleDenotationFlatSet::empty() const {
    return m_data.empty();
}

bool RoleDenotationFlatSet::intersects(const RoleDenotationFlatSet& other) const {
    for (const auto single : m_data) {
        if (other.contains(single)) return true;
    }
    return false;
}

bool RoleDenotationFlatSet::is_subset_of(const RoleDenotationFlatSet& other) const {
    for (const auto single : m_data) {
        if (!other.contains(single)) return false;
    }
    return true;
}

std::vector<std::pair<int, int>> RoleDenotationFlatSet::to_sorted_vector() const {
    std::vector<std::pair<int, int>> result(m_data.begin(), m_data.end());
    std::sort(result.begin(), result.end());
    return result;
}

int RoleDenotationFlatSet::get_num_objects() const {
    return m_num_objects;
}


ConceptDenotationBitset::ConceptDenotationBitset(int num_objects)
    : m_num_objects(num_objects), m_data(dynamic_bitset::DynamicBitset<unsigned>(num_objects)) { }

ConceptDenotationBitset::ConceptDenotationBitset(const ConceptDenotationBitset& other) = default;

ConceptDenotationBitset& ConceptDenotationBitset::operator=(const ConceptDenotationBitset& other) = default;

ConceptDenotationBitset::ConceptDenotationBitset(ConceptDenotationBitset&& other) = default;

ConceptDenotationBitset& ConceptDenotationBitset::operator=(ConceptDenotationBitset&& other) = default;

ConceptDenotationBitset::~ConceptDenotationBitset() = default;

void ConceptDenotationBitset::const_iterator::seek_next() {
    while (++m_index < m_num_objects) {
        if (m_data.test(m_index)) break;
    }
}

ConceptDenotationBitset::const_iterator::const_iterator(
    ConceptDenotationBitset::const_iterator::const_reference data, int num_objects, bool end)
    : m_data(data), m_num_objects(num_objects), m_index(end ? num_objects : -1) {
    if (!end) seek_next();
}

bool ConceptDenotationBitset::const_iterator::operator!=(const const_iterator& other) const {
    return !(*this == other);
}

bool ConceptDenotationBitset::const_iterator::operator==(const const_iterator& other) const {
    return ((m_index == other.m_index) && (&m_data == &other.m_data));
}

const int& ConceptDenotationBitset::const_iterator::operator*() const {
    return m_index;
}

ConceptDenotationBitset::const_iterator ConceptDenotationBitset::const_iterator::operator++(int) {
    ConceptDenotationBitset::const_iterator prev = *this;
    seek_next();
    return prev;
}

ConceptDenotationBitset::const_iterator& ConceptDenotationBitset::const_iterator::operator++() {
    seek_next();
    return *this;
}

ConceptDenotationBitset& ConceptDenotationBitset::operator&=(const ConceptDenotationBitset& other) {
    m_data &= other.m_data;
    return *this;
}

ConceptDenotationBitset& ConceptDenotationBitset::operator|=(const ConceptDenotationBitset& other) {
    m_data |= other.m_data;
    return *this;
}

ConceptDenotationBitset& ConceptDenotationBitset::operator-=(const ConceptDenotationBitset& other) {
    m_data -= other.m_data;
    return *this;
}

ConceptDenotationBitset& ConceptDenotationBitset::operator~() {
    ~m_data;
    return *this;
}

ConceptDenotationBitset::const_iterator ConceptDenotationBitset::begin() const {
    return ConceptDenotationBitset::const_iterator(m_data, m_num_objects);
}

ConceptDenotationBitset::const_iterator ConceptDenotationBitset::end() const {
    return ConceptDenotationBitset::const_iterator(m_data, m_num_objects, true);
}

bool ConceptDenotationBitset::contains(int value) const {
    assert(value >= 0 && value < m_num_objects);
    return m_data.test(value);
}

void ConceptDenotationBitset::insert(int value) {
    assert(value >= 0 && value < m_num_objects);
    m_data.set(value);
}

void ConceptDenotationBitset::erase(int value) {
    assert(value >= 0 && value < m_num_objects);
    m_data.reset(value);
}

int ConceptDenotationBitset::size() const {
    return m_data.count();
}

bool ConceptDenotationBitset::empty() const {
    return m_data.none();
}

bool ConceptDenotationBitset::intersects(const ConceptDenotationBitset& other) const {
    return m_data.intersects(other.m_data);
}

bool ConceptDenotationBitset::is_subset_of(const ConceptDenotationBitset& other) const {
    return m_data.is_subset_of(other.m_data);
}

std::vector<int> ConceptDenotationBitset::to_sorted_vector() const {
    std::vector<int> result;
    result.reserve(m_num_objects);
    for (int i = 0; i < m_num_objects; ++i) {
        if (m_data.test(i)) result.push_back(i);
    }
    result.shrink_to_fit();
    return result;
}

int ConceptDenotationBitset::get_num_objects() const {
    return m_num_objects;
}


RoleDenotationBitset::RoleDenotationBitset(int num_objects)
    : m_num_objects(num_objects), m_data(dynamic_bitset::DynamicBitset<unsigned>(num_objects * num_objects)) { }

RoleDenotationBitset::RoleDenotationBitset(const RoleDenotationBitset& other) = default;

RoleDenotationBitset& RoleDenotationBitset::operator=(const RoleDenotationBitset& other) = default;

RoleDenotationBitset::RoleDenotationBitset(RoleDenotationBitset&& other) = default;

RoleDenotationBitset& RoleDenotationBitset::operator=(RoleDenotationBitset&& other) = default;

RoleDenotationBitset::~RoleDenotationBitset() = default;

void RoleDenotationBitset::const_iterator::seek_next() {
    int& i = m_indices.first;
    int& j = m_indices.second;
    int offset = i * m_num_objects;
    while (i < m_num_objects) {
        ++j;
        if (j == m_num_objects) {  // end of row is reached
            ++i;
            offset += m_num_objects;
            j = 0;
            if (i == m_num_objects) break;  // reached last row
        }
        if (m_data.test(offset + j)) break;
    }
    assert(offset + j <= m_data.size());
}

RoleDenotationBitset::const_iterator::const_iterator(const_reference data, int num_objects, bool end)
    : m_data(data), m_num_objects(num_objects), m_indices(end ? std::pair<size_t, size_t>(num_objects, 0) : std::pair<size_t, size_t>(0, -1)) {
    if (!end) seek_next();
}

bool RoleDenotationBitset::const_iterator::operator!=(const const_iterator& other) const {
    return !(*this == other);
}

bool RoleDenotationBitset::const_iterator::operator==(const const_iterator& other) const {
    return ((m_indices == other.m_indices) && (&m_data == &other.m_data));
}

const std::pair<int, int>& RoleDenotationBitset::const_iterator::operator*() const {
    return m_indices;
}

RoleDenotationBitset::const_iterator RoleDenotationBitset::const_iterator::operator++(int) {
    RoleDenotationBitset::const_iterator prev = *this;
    seek_next();
    return prev;
}

RoleDenotationBitset::const_iterator& RoleDenotationBitset::const_iterator::operator++() {
    seek_next();
    return *this;
}

RoleDenotationBitset& RoleDenotationBitset::operator&=(const RoleDenotationBitset& other) {
    m_data &= other.m_data;
    return *this;
}

RoleDenotationBitset& RoleDenotationBitset::operator|=(const RoleDenotationBitset& other) {
    m_data |= other.m_data;
    return *this;
}

RoleDenotationBitset& RoleDenotationBitset::operator-=(const RoleDenotationBitset& other) {
    m_data -= other.m_data;
    return *this;
}

RoleDenotationBitset& RoleDenotationBitset::operator~() {
    ~m_data;
    return *this;
}

RoleDenotationBitset::const_iterator RoleDenotationBitset::begin() const {
    return RoleDenotationBitset::const_iterator(m_data, m_num_objects);
}

RoleDenotationBitset::const_iterator RoleDenotationBitset::end() const {
    return RoleDenotationBitset::const_iterator(m_data, m_num_objects, true);
}

bool RoleDenotationBitset::contains(const std::pair<int, int>& value) const {
    return m_data.test(value.first * m_num_objects + value.second);
}

void RoleDenotationBitset::insert(const std::pair<int, int>& value) {
    return m_data.set(value.first * m_num_objects + value.second);
}

void RoleDenotationBitset::erase(const std::pair<int, int>& value) {
    return m_data.reset(value.first * m_num_objects + value.second);
}

int RoleDenotationBitset::size() const {
    return m_data.count();
}

bool RoleDenotationBitset::empty() const {
    return m_data.none();
}

bool RoleDenotationBitset::intersects(const RoleDenotationBitset& other) const {
    return m_data.intersects(other.m_data);
}

bool RoleDenotationBitset::is_subset_of(const RoleDenotationBitset& other) const {
    return m_data.is_subset_of(other.m_data);
}

std::vector<std::pair<int, int>> RoleDenotationBitset::to_sorted_vector() const {
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

int RoleDenotationBitset::get_num_objects() const {
    return m_num_objects;
}


InstanceInfo::InstanceInfo(std::shared_ptr<const VocabularyInfo> vocabulary_info) : m_pImpl(InstanceInfoImpl(vocabulary_info)) { }

InstanceInfo::InstanceInfo(const InstanceInfo& other)
    : m_pImpl(*other.m_pImpl) {}

InstanceInfo& InstanceInfo::operator=(const InstanceInfo& other) {
    if (this != &other) {
        *m_pImpl = *other.m_pImpl;
    }
    return *this;
}

InstanceInfo::InstanceInfo(InstanceInfo&& other)
    : m_pImpl(std::move(*other.m_pImpl)) { }

InstanceInfo& InstanceInfo::operator=(InstanceInfo&& other) {
    if (this != &other) {
        std::swap(*m_pImpl, *other.m_pImpl);
    }
    return *this;
}

InstanceInfo::~InstanceInfo() = default;

const Object& InstanceInfo::add_object(const std::string& object_name) {
    return m_pImpl->add_object(object_name);
}

const Atom& InstanceInfo::add_atom(const Predicate& predicate, const std::vector<Object>& objects) {
    return m_pImpl->add_atom(predicate, objects);
}

const Atom& InstanceInfo::add_static_atom(const Predicate& predicate, const std::vector<Object>& objects) {
    return m_pImpl->add_static_atom(predicate, objects);
}

const Atom& InstanceInfo::add_atom(const std::string &name, const Name_Vec &object_names) {
    return m_pImpl->add_atom(name, object_names);
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

const std::vector<Atom>& InstanceInfo::get_static_atoms() const {
    return m_pImpl->get_static_atoms();
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

const phmap::flat_hash_map<int, std::vector<int>>& InstanceInfo::get_per_predicate_idx_static_atom_idxs() const {
    return m_pImpl->get_per_predicate_idx_static_atom_idxs();
}

const ConceptDenotation& InstanceInfo::get_top_concept() const {
    return m_pImpl->get_top_concept();
}

const RoleDenotation& InstanceInfo::get_top_role() const {
    return m_pImpl->get_top_role();
}


VocabularyInfo::VocabularyInfo() : m_pImpl(VocabularyInfoImpl()) { }

VocabularyInfo::VocabularyInfo(const VocabularyInfo& other) : m_pImpl(*other.m_pImpl) { }

VocabularyInfo& VocabularyInfo::operator=(const VocabularyInfo& other) {
    if (this != &other) {
        *m_pImpl = *other.m_pImpl;
    }
    return *this;
}

VocabularyInfo::VocabularyInfo(VocabularyInfo&& other)
    : m_pImpl(std::move(*other.m_pImpl)) { }

VocabularyInfo& VocabularyInfo::operator=(VocabularyInfo&& other) {
    if (this != &other) {
        std::swap(*m_pImpl, *other.m_pImpl);
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


BaseElement::BaseElement(std::shared_ptr<const VocabularyInfo> vocabulary_info, int index)
    : m_vocabulary_info(vocabulary_info), m_index(index) { }

BaseElement::~BaseElement() { }

int BaseElement::get_index() const {
    return m_index;
}

std::shared_ptr<const VocabularyInfo> BaseElement::get_vocabulary_info() const {
    return m_vocabulary_info;
}


Concept::Concept(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const element::Concept>&& concept, int index)
    : Element<ConceptDenotation>(vocabulary_info, index), m_element(concept) {
    if (!m_element) {
        throw std::runtime_error("Concept::Concept - tried to construct Concept from nullptr");
    }
}

Concept::Concept(const Concept& other) = default;

Concept& Concept::operator=(const Concept& other) = default;

Concept::Concept(Concept&& other) = default;

Concept& Concept::operator=(Concept&& other) = default;

Concept::~Concept() = default;

ConceptDenotation Concept::evaluate(const State& state) const {
    if (state.get_instance_info()->get_vocabulary_info() != get_vocabulary_info()) {
        throw std::runtime_error("Concept::evaluate - mismatched vocabularies of Concept and State.");
    }
    return m_element->evaluate(state);
}

int Concept::compute_complexity() const {
    return m_element->compute_complexity();
}

std::string Concept::compute_repr() const {
    return m_element->compute_repr();
}

std::shared_ptr<const element::Concept> Concept::get_element() const {
    return m_element;
}


Role::Role(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const element::Role>&& role, int index)
    : Element<RoleDenotation>(vocabulary_info, index), m_element(role) {
    if (!m_element) {
        throw std::runtime_error("Role::Role - tried to construct Role from nullptr");
    }
}

Role::Role(const Role& other) = default;

Role& Role::operator=(const Role& other) = default;

Role::Role(Role&& other) = default;

Role& Role::operator=(Role&& other) = default;

Role::~Role() = default;

RoleDenotation Role::evaluate(const State& state) const {
    if (state.get_instance_info()->get_vocabulary_info() != get_vocabulary_info()) {
        throw std::runtime_error("Role::evaluate - mismatched vocabularies of Role and State.");
    }
    return m_element->evaluate(state);
}

int Role::compute_complexity() const {
    return m_element->compute_complexity();
}

std::string Role::compute_repr() const {
    return m_element->compute_repr();
}

std::shared_ptr<const element::Role> Role::get_element() const {
    return m_element;
}


Numerical::Numerical(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const element::Numerical>&& numerical, int index)
    : Element<int>(vocabulary_info, index), m_element(numerical) {
    if (!m_element) {
        throw std::runtime_error("Numerical::Numerical - tried to construct Numerical from nullptr");
    }
}

Numerical::Numerical(const Numerical& other) = default;

Numerical& Numerical::operator=(const Numerical& other) = default;

Numerical::Numerical(Numerical&& other) = default;

Numerical& Numerical::operator=(Numerical&& other) = default;

Numerical::~Numerical() = default;

int Numerical::evaluate(const State& state) const {
    if (state.get_instance_info()->get_vocabulary_info() != get_vocabulary_info()) {
        throw std::runtime_error("Numerical::evaluate - mismatched vocabularies of Numerical and State.");
    }
    return m_element->evaluate(state);
}

int Numerical::compute_complexity() const {
    return m_element->compute_complexity();
}

std::string Numerical::compute_repr() const {
    return m_element->compute_repr();
}

std::shared_ptr<const element::Numerical> Numerical::get_element() const {
    return m_element;
}


Boolean::Boolean(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const element::Boolean>&& boolean, int index)
    : Element<bool>(vocabulary_info, index), m_element(boolean) {
    if (!m_element) {
        throw std::runtime_error("Boolean::Boolean - tried to construct Boolean from nullptr");
    }
}

Boolean::Boolean(const Boolean& other) = default;

Boolean& Boolean::operator=(const Boolean& other) = default;

Boolean::Boolean(Boolean&& other) = default;

Boolean& Boolean::operator=(Boolean&& other) = default;

Boolean::~Boolean() = default;

bool Boolean::evaluate(const State& state) const {
    if (state.get_instance_info()->get_vocabulary_info() != get_vocabulary_info()) {
        throw std::runtime_error("Boolean::evaluate - mismatched vocabularies of Boolean and State.");
    }
    return m_element->evaluate(state);
}

int Boolean::compute_complexity() const {
    return m_element->compute_complexity();
}

std::string Boolean::compute_repr() const {
    return m_element->compute_repr();
}

std::shared_ptr<const element::Boolean> Boolean::get_element() const {
    return m_element;
}


SyntacticElementFactory::SyntacticElementFactory(std::shared_ptr<const VocabularyInfo> vocabulary_info) : m_pImpl(SyntacticElementFactoryImpl(vocabulary_info)) { }

SyntacticElementFactory::SyntacticElementFactory(const SyntacticElementFactory& other) : m_pImpl(*other.m_pImpl) { }

SyntacticElementFactory& SyntacticElementFactory::operator=(const SyntacticElementFactory& other) {
    if (this != &other) {
        *m_pImpl = *other.m_pImpl;
    }
    return *this;
}

SyntacticElementFactory::SyntacticElementFactory(SyntacticElementFactory&& other)
    : m_pImpl(std::move(*other.m_pImpl)) { }

SyntacticElementFactory& SyntacticElementFactory::operator=(SyntacticElementFactory&& other) {
    if (this != &other) {
        std::swap(*m_pImpl, *other.m_pImpl);
    }
    return *this;
}

SyntacticElementFactory::~SyntacticElementFactory() = default;

const VocabularyInfo* SyntacticElementFactory::get_vocabulary_info() const {
    return m_pImpl->get_vocabulary_info();
}

Concept SyntacticElementFactory::parse_concept(const std::string &description, int index) {
    return m_pImpl->parse_concept(description);
}

Role SyntacticElementFactory::parse_role(const std::string &description, int index) {
    return m_pImpl->parse_role(description, index);
}

Numerical SyntacticElementFactory::parse_numerical(const std::string &description, int index) {
    return m_pImpl->parse_numerical(description, index);
}

Boolean SyntacticElementFactory::parse_boolean(const std::string &description, int index) {
    return m_pImpl->parse_boolean(description, index);
}


Boolean SyntacticElementFactory::make_empty_boolean(const Concept& concept, int index) {
    return m_pImpl->make_empty_boolean(concept, index);
}

Boolean SyntacticElementFactory::make_empty_boolean(const Role& role, int index) {
    return m_pImpl->make_empty_boolean(role, index);
}

Boolean SyntacticElementFactory::make_concept_inclusion_boolean(const Concept& concept_left, const Concept& concept_right, int index) {
    return m_pImpl->make_concept_inclusion_boolean(concept_left, concept_right, index);
}

Boolean SyntacticElementFactory::make_role_inclusion_boolean(const Role& role_left, const Role& role_right, int index) {
    return m_pImpl->make_role_inclusion_boolean(role_left, role_right, index);
}

Boolean SyntacticElementFactory::make_nullary_boolean(const Predicate& predicate, int index) {
    return m_pImpl->make_nullary_boolean(predicate, index);
}

Concept SyntacticElementFactory::make_all_concept(const Role& role, const Concept& concept, int index) {
    return m_pImpl->make_all_concept(role, concept, index);
}

Concept SyntacticElementFactory::make_and_concept(const Concept& concept_left, const Concept& concept_right, int index) {
    return m_pImpl->make_and_concept(concept_left, concept_right, index);
}

Concept SyntacticElementFactory::make_bot_concept(int index) {
    return m_pImpl->make_bot_concept(index);
}

Concept SyntacticElementFactory::make_diff_concept(const Concept& concept_left, const Concept& concept_right, int index) {
    return m_pImpl->make_diff_concept(concept_left, concept_right, index);
}

Concept SyntacticElementFactory::make_equal_concept(const Role& role_left, const Role& role_right, int index) {
    return m_pImpl->make_equal_concept(role_left, role_right, index);
}

Concept SyntacticElementFactory::make_not_concept(const Concept& concept, int index) {
    return m_pImpl->make_not_concept(concept, index);
}

Concept SyntacticElementFactory::make_one_of_concept(const Constant& constant, int index) {
    return m_pImpl->make_one_of_concept(constant, index);
}

Concept SyntacticElementFactory::make_or_concept(const Concept& concept_left, const Concept& concept_right, int index) {
    return m_pImpl->make_or_concept(concept_left, concept_right, index);
}

Concept SyntacticElementFactory::make_projection_concept(const Role& role, int pos, int index) {
    return m_pImpl->make_projection_concept(role, pos, index);
}

Concept SyntacticElementFactory::make_primitive_concept(const Predicate& predicate, int pos, int index) {
    return m_pImpl->make_primitive_concept(predicate, pos, index);
}

Concept SyntacticElementFactory::make_some_concept(const Role& role, const Concept& concept, int index) {
    return m_pImpl->make_some_concept(role, concept, index);
}

Concept SyntacticElementFactory::make_subset_concept(const Role& role_left, const Role& role_right, int index) {
    return m_pImpl->make_subset_concept(role_left, role_right, index);
}

Concept SyntacticElementFactory::make_top_concept(int index) {
    return m_pImpl->make_top_concept(index);
}

Numerical SyntacticElementFactory::make_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to, int index) {
    return m_pImpl->make_concept_distance(concept_from, role, concept_to, index);
}

Numerical SyntacticElementFactory::make_count(const Concept& concept, int index) {
    return m_pImpl->make_count(concept, index);
}

Numerical SyntacticElementFactory::make_count(const Role& role, int index) {
    return m_pImpl->make_count(role, index);
}

Numerical SyntacticElementFactory::make_role_distance(const Role& role_from, const Role& role, const Role& role_to, int index) {
    return m_pImpl->make_role_distance(role_from, role, role_to, index);
}

Numerical SyntacticElementFactory::make_sum_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to, int index) {
    return m_pImpl->make_sum_concept_distance(concept_from, role, concept_to, index);
}

Numerical SyntacticElementFactory::make_sum_role_distance(const Role& role_from, const Role& role, const Role& role_to, int index) {
    return m_pImpl->make_sum_role_distance(role_from, role, role_to, index);
}

Role SyntacticElementFactory::make_and_role(const Role& role_left, const Role& role_right, int index) {
    return m_pImpl->make_and_role(role_left, role_right, index);
}

Role SyntacticElementFactory::make_compose_role(const Role& role_left, const Role& role_right, int index) {
    return m_pImpl->make_compose_role(role_left, role_right, index);
}

Role SyntacticElementFactory::make_diff_role(const Role& role_left, const Role& role_right, int index) {
    return m_pImpl->make_diff_role(role_left, role_right, index);
}

Role SyntacticElementFactory::make_identity_role(const Concept& concept, int index) {
    return m_pImpl->make_identity_role(concept, index);
}

Role SyntacticElementFactory::make_inverse_role(const Role& role, int index) {
    return m_pImpl->make_inverse_role(role, index);
}

Role SyntacticElementFactory::make_not_role(const Role& role, int index) {
    return m_pImpl->make_not_role(role, index);
}

Role SyntacticElementFactory::make_or_role(const Role& role_left, const Role& role_right, int index) {
    return m_pImpl->make_or_role(role_left, role_right, index);
}

Role SyntacticElementFactory::make_primitive_role(const Predicate& predicate, int pos_1, int pos_2, int index) {
    return m_pImpl->make_primitive_role(predicate, pos_1, pos_2, index);
}

Role SyntacticElementFactory::make_restrict_role(const Role& role, const Concept& concept, int index) {
    return m_pImpl->make_restrict_role(role, concept, index);
}

Role SyntacticElementFactory::make_top_role(int index) {
    return m_pImpl->make_top_role(index);
}

Role SyntacticElementFactory::make_transitive_closure(const Role& role, int index) {
    return m_pImpl->make_transitive_closure(role, index);
}

Role SyntacticElementFactory::make_transitive_reflexive_closure(const Role& role, int index) {
    return m_pImpl->make_transitive_reflexive_closure(role, index);
}

}
