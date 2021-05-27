#ifndef DLP_SRC_CORE_ATOM_H_
#define DLP_SRC_CORE_ATOM_H_

#include <string>
#include <vector>

#include "types.h"

namespace dlp {
namespace core {
class InstanceInfoImpl;

struct AtomImpl {
    // Non-owning parent pointer
    const std::shared_ptr<InstanceInfoImpl> m_parent;
    // Data members
    const int m_atom_idx;
    const std::string m_predicate_name;
    const int m_predicate_idx;
    const Name_Vec m_object_names;
    const Index_Vec m_object_idxs;
    const bool m_is_static;

    AtomImpl(
        std::shared_ptr<InstanceInfoImpl> parent,
        const int atom_idx,
        const std::string &predicate_name,
        int predicate_idx,
        const Name_Vec &object_names,
        const Index_Vec &object_idxs,
        bool is_static);

    int predicate_idx() const;
    int object_idx(int pos) const;
};

}
}

#endif
