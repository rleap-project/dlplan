#include "atom.h"

namespace dlp {
namespace core {

Atom::Atom(
    const std::string &predicate_name,
    int predicate_idx,
    const Name_Vec &object_names,
    const Index_Vec &object_idxs,
    bool constant)
    : m_predicate_name(predicate_name), m_predicate_idx(predicate_idx),
      m_object_names(object_names), m_object_idxs(object_idxs), m_constant(constant) { }

}
}
