#ifndef DLP_SRC_CORE_ATOM_H_
#define DLP_SRC_CORE_ATOM_H_

#include <string>
#include <vector>

#include "types.h"

namespace dlp {
namespace core {

struct Atom {
    const std::string m_predicate_name;
    int m_predicate_idx;
    const Name_Vec m_object_names;
    const Index_Vec m_object_idxs;
    bool m_constant;

    Atom(const std::string &predicate_name,
        int predicate_idx,
        const Name_Vec &object_names,
        const Index_Vec &object_idxs,
        bool constant);
};

}
}

#endif
