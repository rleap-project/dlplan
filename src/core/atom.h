#ifndef DLP_SRC_CORE_ATOM_H_
#define DLP_SRC_CORE_ATOM_H_

#include <string>
#include <vector>

#include "types.h"

namespace dlp {
namespace core {
class InstanceInfoImpl;

struct AtomImpl {
    // Cannot have parent pointer to instance here since atom are stored in instance.
    // A parent pointer is not necessary anyways since an Atom only acts as storage
    // and wont be passed back into the library by the user (as opposed to a State).
    // Data members
    const std::string m_atom_name;
    const int m_atom_idx;
    const std::string m_predicate_name;
    const int m_predicate_idx;
    const Name_Vec m_object_names;
    const Index_Vec m_object_idxs;
    bool m_is_static;

    AtomImpl(
        const std::string& atom_name,
        const int atom_idx,
        const std::string &predicate_name,
        int predicate_idx,
        const Name_Vec &object_names,
        const Index_Vec &object_idxs,
        bool is_static);
    ~AtomImpl() = default;

    int get_predicate_idx() const;
    int get_object_idx(int pos) const;
};

}
}

#endif
