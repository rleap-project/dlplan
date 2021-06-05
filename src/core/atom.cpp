#include "atom.h"

#include <cassert>

#include "../utils/collections.h"


namespace dlp {
namespace core {

AtomImpl::AtomImpl(
    const InstanceInfoImpl& instance_info,
    const std::string& atom_name,
    const int atom_idx,
    const std::string &predicate_name,
    int predicate_idx,
    const Name_Vec &object_names,
    const Index_Vec &object_idxs,
    bool is_static)
    : m_instance_info(&instance_info), m_atom_name(atom_name), m_atom_idx(atom_idx),
      m_predicate_name(predicate_name), m_predicate_idx(predicate_idx),
      m_object_names(object_names), m_object_idxs(object_idxs), m_is_static(is_static) { }

const InstanceInfoImpl* AtomImpl::get_instance_info() const {
    return m_instance_info;
}

const std::string& AtomImpl::get_atom_name() const {
    return m_atom_name;
}

int AtomImpl::get_atom_idx() const {
    return m_atom_idx;
}

const std::string& AtomImpl::get_predicate_name() const {
    return m_predicate_name;
}

int AtomImpl::get_predicate_idx() const {
    return m_predicate_idx;
}

const Name_Vec& AtomImpl::get_object_names() const {
    return m_object_names;
}

const std::string& AtomImpl::get_object_name(int pos) const {
    assert(utils::in_bounds(pos, m_object_names));
    return m_object_names[pos];
}

const Index_Vec& AtomImpl::get_object_idxs() const {
    return m_object_idxs;
}

int AtomImpl::get_object_idx(int pos) const {
    assert(utils::in_bounds(pos, m_object_idxs));
    return m_object_idxs[pos];
}

bool AtomImpl::get_is_static() const {
    return m_is_static;
}

}
}
