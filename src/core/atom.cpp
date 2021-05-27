#include "atom.h"

namespace dlp {
namespace core {

AtomImpl::AtomImpl(
    std::shared_ptr<InstanceInfoImpl> parent,
    const int atom_idx,
    const std::string &predicate_name,
    int predicate_idx,
    const Name_Vec &object_names,
    const Index_Vec &object_idxs,
    bool is_static)
    : m_parent(parent), m_atom_idx(atom_idx),
      m_predicate_name(predicate_name), m_predicate_idx(predicate_idx),
      m_object_names(object_names), m_object_idxs(object_idxs), m_is_static(is_static) { }

}
}
