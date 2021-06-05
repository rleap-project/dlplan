#include "atom.h"

#include <cassert>

#include "../utils/collections.h"


namespace dlp {
namespace core {

AtomImpl::AtomImpl(
    const InstanceInfoImpl& instance_info,
    const std::string& atom_name,
    const int atom_idx,
    const Predicate& predicate,
    const std::vector<Object> &objects,
    bool is_static)
    : m_instance_info(&instance_info), m_atom_name(atom_name), m_atom_idx(atom_idx),
      m_predicate(predicate), m_objects(objects), m_is_static(is_static) { }

const InstanceInfoImpl* AtomImpl::get_instance_info() const {
    return m_instance_info;
}

const std::string& AtomImpl::get_atom_name() const {
    return m_atom_name;
}

int AtomImpl::get_atom_idx() const {
    return m_atom_idx;
}

const Predicate& AtomImpl::get_predicate() const {
    return m_predicate;
}

const std::vector<Object>& AtomImpl::get_objects() const {
    return m_objects;
}

const Object& AtomImpl::get_object(int pos) const {
    assert(utils::in_bounds(pos, m_objects));
    return m_objects[pos];
}


bool AtomImpl::get_is_static() const {
    return m_is_static;
}

}
}
