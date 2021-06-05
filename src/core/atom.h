#ifndef DLP_SRC_CORE_ATOM_H_
#define DLP_SRC_CORE_ATOM_H_

#include <string>
#include <vector>
#include <memory>

#include "../../include/dlp/core.h"
#include "types.h"


namespace dlp {
namespace core {
class InstanceInfoImpl;

class AtomImpl {
private:
    const InstanceInfoImpl* m_instance_info;
    // Data members
    const std::string m_atom_name;
    const int m_atom_idx;
    const Predicate m_predicate;
    const std::vector<Object> m_objects;
    const bool m_is_static;

public:
    AtomImpl(
        const InstanceInfoImpl& instance_info,
        const std::string& atom_name,
        const int atom_idx,
        const Predicate& predicate,
        const std::vector<Object> &objects,
        bool is_static);
    ~AtomImpl() = default;

    const InstanceInfoImpl* get_instance_info() const;
    const std::string& get_atom_name() const;
    int get_atom_idx() const;
    const Predicate& get_predicate() const;
    const std::vector<Object>& get_objects() const;
    const Object& get_object(int pos) const;
    bool get_is_static() const;
};

}
}

#endif
