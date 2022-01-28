#ifndef DLPLAN_SRC_CORE_STATE_H_
#define DLPLAN_SRC_CORE_STATE_H_

#include <string>
#include <vector>
#include <memory>

#include "types.h"


namespace dlplan::core {
class InstanceInfo;
class Atom;

class StateImpl {
private:
    int m_index;
    const std::shared_ptr<const InstanceInfo> m_instance_info;
    // The underlying implementation currently is a vector of atoms.
    const Index_Vec m_atom_idxs;

public:
    StateImpl(std::shared_ptr<const InstanceInfo> instance_info, const std::vector<Atom>& atoms);
    StateImpl(std::shared_ptr<const InstanceInfo> instance_info, const Index_Vec& atom_idxs);
    ~StateImpl() = default;

    /**
     * Getters.
     */
    std::shared_ptr<const InstanceInfo> get_instance_info() const;
    const Index_Vec& get_atom_idxs() const;

    int get_index() const;
    void set_index(int index);

    std::string str() const;

    size_t compute_hash() const;
};

}

#endif
