#include "instance_info.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <cassert>

#include "state.h"
#include "../utils/collections.h"


namespace dlp {
namespace core {

static bool exists(const std::string& name, std::unordered_map<std::string, unsigned>& mapping) {
    auto f = mapping.find(name);
    return (f != mapping.end());
}

InstanceInfoImpl::InstanceInfoImpl(const VocabularyInfoImpl& vocabulary_info)
    : m_vocabulary_info(vocabulary_info.shared_from_this()) {}

AtomImpl InstanceInfoImpl::add_atom(const std::string &predicate_name, const Name_Vec &object_names) {
    if (!m_vocabulary_info->exists_predicate_name(predicate_name)) {
        throw std::runtime_error("InstanceInfoImpl::add_atom - name of predicate missing in vocabulary ("s + predicate_name + ")");
    } else if (m_vocabulary_info->get_predicate(m_vocabulary_info->get_predicate_idx(predicate_name)).m_arity != object_names.size()) {
        throw std::runtime_error("InstanceInfoImpl::add_atom - arity of predicate in vocabulary does not match with atom ("s + std::to_string(m_vocabulary_info->get_predicate(m_vocabulary_info->get_predicate_idx(predicate_name)).m_arity) + " != " + std::to_string(object_names.size()));
    }
    // predicate related
    bool predicate_exists = exists(predicate_name, m_predicate_name_to_predicate_idx);
    unsigned predicate_idx;
    if (!predicate_exists) {
        predicate_idx = m_predicate_name_to_predicate_idx.size();
        m_predicate_name_to_predicate_idx.emplace(predicate_name, predicate_idx);
    } else {
        predicate_idx = m_predicate_name_to_predicate_idx.at(predicate_name);
    }
    // object related
    std::stringstream ss;
    ss << predicate_name << "(";
    Index_Vec object_idxs;
    for (unsigned i = 0; i < object_names.size(); ++i) {
        const std::string& object_name = object_names[i];
        bool object_exists = exists(object_name, m_object_name_to_object_idx);
        unsigned object_idx;
        if (!object_exists) {
            object_idx = m_objects.size();
            m_objects.emplace_back(*this, object_name, object_idx);
            m_object_name_to_object_idx.emplace(object_name, object_idx);
        } else {
            object_idx = m_object_name_to_object_idx.at(object_name);
        }
        object_idxs.push_back(object_idx);
        ss << object_name;
        if (i < object_names.size() - 1) {
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

    m_atoms.emplace_back(*this, atom_name, atom_idx, predicate_name, predicate_idx, object_names, object_idxs, false);
    return m_atoms.back();
}

AtomImpl InstanceInfoImpl::add_static_atom(const std::string& predicate_name, const Name_Vec& object_names) {
    const AtomImpl& atom = add_atom(predicate_name, object_names);
    m_atoms[atom.m_atom_idx].m_is_static = true;
    m_static_atom_idxs.push_back(atom.m_atom_idx);
    return atom;
}

StateImpl InstanceInfoImpl::parse_state(const Name_Vec& atom_names) const {
    Index_Vec atoms;
    atoms.reserve(atom_names.size() + m_static_atom_idxs.size());
    for (const auto& atom_name : atom_names) {
        auto p = m_atom_name_to_atom_idx.find(atom_name);
        if (p == m_atom_name_to_atom_idx.end()) {
            throw std::runtime_error("InstanceInfoImpl::parse_state - atom name ("s + atom_name + ") not found in instance.");
        }
        atoms.push_back(p->second);
    }
    atoms.insert(atoms.end(), m_static_atom_idxs.begin(), m_static_atom_idxs.end());
    return StateImpl(*this, std::move(atoms));
}

StateImpl InstanceInfoImpl::convert_state(const std::vector<Atom>& atoms) const {
    // Ensure in the parent call that parent pointer of atom and info are identical.
    assert(std::all_of(atoms.begin(), atoms.end(), [&](const Atom& atom){ return utils::in_bounds(atom.get_atom_idx(), m_atoms); }));
    Index_Vec atom_indices;
    atom_indices.reserve(atoms.size() + m_static_atom_idxs.size());
    for (const auto& atom : atoms) {
        atom_indices.push_back(atom.get_atom_idx());
    }
    atom_indices.insert(atom_indices.end(), m_static_atom_idxs.begin(), m_static_atom_idxs.end());
    return StateImpl(*this, std::move(atom_indices));
}

StateImpl InstanceInfoImpl::convert_state(const Index_Vec& atom_idxs) const {
    if (!std::all_of(atom_idxs.begin(), atom_idxs.end(), [&](int atom_idx){ return utils::in_bounds(atom_idx, m_atoms); })) {
        throw std::runtime_error("InstanceInfoImpl::convert_state - atom index out of range.");
    }
    Index_Vec atom_indices;
    atom_indices.reserve(atom_idxs.size() + m_static_atom_idxs.size());
    atom_indices.insert(atom_indices.end(), atom_idxs.begin(), atom_idxs.end());
    atom_indices.insert(atom_indices.end(), m_static_atom_idxs.begin(), m_static_atom_idxs.end());
    return StateImpl(*this, std::move(atom_indices));
}

const AtomImpl& InstanceInfoImpl::get_atom(unsigned atom_idx) const {
    if (!utils::in_bounds(atom_idx, m_atoms)) {
        throw std::runtime_error("InstanceInfoImpl::get_atom - atom index out of range.");
    }
    return m_atoms[atom_idx];
}

const std::vector<AtomImpl>& InstanceInfoImpl::get_atoms() const {
    return m_atoms;
}

unsigned InstanceInfoImpl::get_num_objects() const {
    return m_object_name_to_object_idx.size();
}

unsigned InstanceInfoImpl::get_object_idx(const std::string& object_name) const {
    if (m_object_name_to_object_idx.find(object_name) == m_object_name_to_object_idx.end()) {
        throw std::runtime_error("InstanceInfoImpl::get_object_idx - no object with name ("s + object_name + ").");
    }
    return m_object_name_to_object_idx.at(object_name);
}

const ObjectImpl& InstanceInfoImpl::get_object(unsigned object_idx) const {
    if (!utils::in_bounds(object_idx, m_objects)) {
        throw std::runtime_error("InstanceInfoImpl::get_object - object index out of range.");
    }
    return m_objects[object_idx];
}

const std::shared_ptr<const VocabularyInfoImpl>& InstanceInfoImpl::get_vocabulary_info() const {
    return m_vocabulary_info;
}

}
}
