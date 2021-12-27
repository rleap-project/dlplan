#include "instance_info.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <cassert>

#include "vocabulary_info.h"
#include "object.h"
#include "atom.h"
#include "state.h"
#include "../utils/collections.h"


namespace dlplan::core {

static bool exists(const std::string& name, std::unordered_map<std::string, unsigned>& mapping) {
    auto f = mapping.find(name);
    return (f != mapping.end());
}

InstanceInfoImpl::InstanceInfoImpl(std::shared_ptr<const VocabularyInfo> vocabulary_info)
    : m_vocabulary_info(vocabulary_info), m_top_concept(ConceptDenotation(0)), m_top_role(RoleDenotation(0)) {
}

const Atom& InstanceInfoImpl::add_atom(const std::string &predicate_name, const Name_Vec &object_names, bool negated, bool is_static) {
    if (!m_vocabulary_info->exists_predicate_name(predicate_name)) {
        throw std::runtime_error("InstanceInfoImpl::add_atom - name of predicate missing in vocabulary ("s + predicate_name + ")");
    } else if (m_vocabulary_info->get_predicate(m_vocabulary_info->get_predicate_idx(predicate_name)).get_arity() != static_cast<int>(object_names.size())) {
        throw std::runtime_error("InstanceInfoImpl::add_atom - arity of predicate in vocabulary does not match with atom ("s + std::to_string(m_vocabulary_info->get_predicate(m_vocabulary_info->get_predicate_idx(predicate_name)).get_arity()) + " != " + std::to_string(object_names.size()));
    }
    // predicate related
    int predicate_idx = m_vocabulary_info->get_predicate_idx(predicate_name);
    const Predicate& predicate = m_vocabulary_info->get_predicate(predicate_idx);
    // object related
    std::stringstream ss;
    if (negated) ss << "not ";
    ss << predicate_name << "(";
    std::vector<Object> objects;
    for (int i = 0; i < static_cast<int>(object_names.size()); ++i) {
        const std::string& object_name = object_names[i];
        bool object_exists = exists(object_name, m_object_name_to_object_idx);
        int object_idx;
        if (!object_exists) {
            object_idx = m_objects.size();
            m_objects.push_back(Object(object_name, object_idx));
            m_object_name_to_object_idx.emplace(object_name, object_idx);
        } else {
            object_idx = m_object_name_to_object_idx.at(object_name);
        }
        objects.push_back(Object(object_name, object_idx));
        ss << object_name;
        if (i < static_cast<int>(object_names.size()) - 1) {
            ss << ",";
        }
    }
    ss << ")";
    std::string atom_name = ss.str();
    if (m_atom_name_to_atom_idx.find(atom_name) != m_atom_name_to_atom_idx.end()) {
        throw std::runtime_error("InstanceInfoImpl::add_atom - adding duplicate atom with name ("s + atom_name + ") is not allowed.");
    }
    // atom related
    int atom_idx = m_atoms.size();
    m_atom_name_to_atom_idx.emplace(atom_name, m_atoms.size());
    if (is_static) {
        m_static_atom_idxs.push_back(atom_idx);
    }

    m_atoms.push_back(Atom(atom_name, atom_idx, predicate, objects, is_static));
    return m_atoms.back();
}

const Atom& InstanceInfoImpl::add_atom(const std::string &predicate_name, const Name_Vec &object_names, bool negated) {
    return add_atom(predicate_name, object_names, negated, false);
}

const Atom& InstanceInfoImpl::add_static_atom(const std::string& predicate_name, const Name_Vec& object_names) {
    return add_atom(predicate_name, object_names, false, true);
}

bool InstanceInfoImpl::exists_atom(const Atom& atom) const {
    if (!utils::in_bounds(atom.get_index(), m_atoms)) {
        throw std::runtime_error("InstanceInfoImpl::exists_atom: atom index out of range.");
    }
    // we only need to check the position with the corresponding index.
    return (m_atoms[atom.get_index()] == atom) ? true : false;
}

const std::vector<Atom>& InstanceInfoImpl::get_atoms() const {
    return m_atoms;
}

const Atom& InstanceInfoImpl::get_atom(int atom_idx) const {
    if (!utils::in_bounds(atom_idx, m_atoms)) {
        throw std::runtime_error("InstanceInfoImpl::get_atom - atom index out of range.");
    }
    return m_atoms[atom_idx];
}

int InstanceInfoImpl::get_atom_idx(const std::string& name) const {
    if (m_atom_name_to_atom_idx.find(name) == m_atom_name_to_atom_idx.end()) {
        throw std::runtime_error("InstanceInfoImpl::get_atom_idx - no atom with name ("s + name + ").");
    }
    return m_atom_name_to_atom_idx.at(name);
}

bool InstanceInfoImpl::exists_object(const Object& object) const {
    if (!utils::in_bounds(object.get_index(), m_objects)) {
        throw std::runtime_error("InstanceInfoImpl::exists_object: object index out of range.");
    }
    // we only need to check the position with the corresponding index.
    return (m_objects[object.get_index()] == object) ? true : false;
}

bool InstanceInfoImpl::exists_object(const std::string name) const {
    return m_object_name_to_object_idx.find(name) != m_object_name_to_object_idx.end();
}

const std::vector<Object>& InstanceInfoImpl::get_objects() const {
    return m_objects;
}

const Object& InstanceInfoImpl::get_object(int object_idx) const {
    if (!utils::in_bounds(object_idx, m_objects)) {
        throw std::runtime_error("InstanceInfoImpl::get_object - object index out of range.");
    }
    return m_objects[object_idx];
}

int InstanceInfoImpl::get_object_idx(const std::string& object_name) const {
    if (m_object_name_to_object_idx.find(object_name) == m_object_name_to_object_idx.end()) {
        throw std::runtime_error("InstanceInfoImpl::get_object_idx - no object with name ("s + object_name + ").");
    }
    return m_object_name_to_object_idx.at(object_name);
}

int InstanceInfoImpl::get_num_objects() const {
    return m_objects.size();
}

std::shared_ptr<const VocabularyInfo> InstanceInfoImpl::get_vocabulary_info() const {
    return m_vocabulary_info;
}

const Index_Vec& InstanceInfoImpl::get_static_atom_idxs() const {
    return m_static_atom_idxs;
}

const ConceptDenotation& InstanceInfoImpl::get_top_concept() const {
    if (static_cast<int>(m_top_concept.get_const_data().size()) != get_num_objects()) {
        m_top_concept = ConceptDenotation(get_num_objects());
        // for hashing, we want to ensure that the bits outside the range remain 0.
        for (int i = 0; i < get_num_objects(); ++i) {
            m_top_concept.get_data().set(i);
        }
    }
    return m_top_concept;
}

const RoleDenotation& InstanceInfoImpl::get_top_role() const {
    if (static_cast<int>(m_top_role.get_const_data().size()) != get_num_objects() * get_num_objects()) {
        m_top_role = RoleDenotation(get_num_objects() * get_num_objects());
        // for hashing, we want to ensure that the bits outside the range remain 0.
        for (int i = 0; i < get_num_objects() * get_num_objects(); ++i) {
            m_top_role.get_data().set(i);
        }
    }
    return m_top_role;
}

}
