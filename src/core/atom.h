#ifndef DLP_SRC_CORE_ATOM_H_
#define DLP_SRC_CORE_ATOM_H_

#include <string>
#include <vector>
#include <memory>

#include "types.h"


namespace dlp {
namespace core {
class InstanceInfoImpl;

struct AtomImpl {
    const InstanceInfoImpl* m_instance_info;
    // Data members
    const std::string m_atom_name;
    const int m_atom_idx;
    const std::string m_predicate_name;
    const int m_predicate_idx;
    const Name_Vec m_object_names;
    const Index_Vec m_object_idxs;
    bool m_is_static;

    AtomImpl(
        const InstanceInfoImpl& instance_info,
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
    const InstanceInfoImpl* get_instance_info() const;
};

}
}

#endif
