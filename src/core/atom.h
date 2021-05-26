#ifndef DLP_SRC_CORE_ATOM_H_
#define DLP_SRC_CORE_ATOM_H_

#include <string>
#include <vector>

#include "types.h"

namespace dlp {
namespace core {

struct AtomImpl {
    const int m_atom_idx;
    const std::string m_predicate_name;
    const int m_predicate_idx;
    const Name_Vec m_object_names;
    const Index_Vec m_object_idxs;
    const bool m_is_static;

    AtomImpl(const int atom_idx,
        const std::string &predicate_name,
        int predicate_idx,
        const Name_Vec &object_names,
        const Index_Vec &object_idxs,
        bool is_static);
};

}
}

#endif