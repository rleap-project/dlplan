#include "instance_info.h"
#include "state.h"

#include <algorithm>
#include <iostream>


namespace dlp {
namespace core {

static bool exists(const std::string& name, std::unordered_map<std::string, unsigned>& mapping) {
    auto f = mapping.find(name);
    return (f != mapping.end());
}

static unsigned insert_or_retrieve(const std::string& name, std::unordered_map<std::string, unsigned>& mapping) {
    auto f = mapping.find(name);
    if (f == mapping.end()) {
        mapping.emplace(std::make_pair(name, mapping.size()));
        return mapping.size() - 1;
    }
    return f->second;
}

AtomImpl InstanceInfoImpl::add_atom(const std::string &predicate_name, const Name_Vec &object_names, bool is_static) {
    bool predicate_exists = exists(predicate_name, m_predicate_name_to_predicate_idx);
    if (!predicate_exists) {
        m_predicate_arities.push_back(object_names.size());
    }
    int atom_idx = m_atoms.size();
    int predicate_idx = insert_or_retrieve(predicate_name, m_predicate_name_to_predicate_idx);
    Index_Vec object_idxs;
    for (const std::string& object_name : object_names) {
        object_idxs.push_back(insert_or_retrieve(object_name, m_object_name_to_object_idx));
    }
    if (is_static) {
        m_static_atom_idxs.push_back(atom_idx);
    }
    m_atoms.push_back(AtomImpl(atom_idx, predicate_name, predicate_idx, object_names, object_idxs, is_static));
    return m_atoms.back();
}

StateImpl InstanceInfoImpl::convert_state(std::shared_ptr<InstanceInfoImpl> info, const Index_Vec& atom_idxs) {
    Index_Vec atoms;
    atoms.reserve(atom_idxs.size() + m_static_atom_idxs.size());
    atoms.insert(atoms.end(), atom_idxs.begin(), atom_idxs.end());
    atoms.insert(atoms.end(), m_static_atom_idxs.begin(), m_static_atom_idxs.end());
    return StateImpl(info, std::move(atoms));
}

bool InstanceInfoImpl::exists_predicate_name(const std::string& name) const {
    return m_predicate_name_to_predicate_idx.find(name) != m_predicate_name_to_predicate_idx.end();
}

unsigned InstanceInfoImpl::predicate_idx(const std::string& name) const {
    return m_predicate_name_to_predicate_idx.at(name);
}

unsigned InstanceInfoImpl::predicate_arity(unsigned predicate_idx) const {
    return m_predicate_arities[predicate_idx];
}

const AtomImpl& InstanceInfoImpl::atom(unsigned atom_idx) const {
    return m_atoms[atom_idx];
}

}
}
