#ifndef DLP_SRC_CORE_STATE_H_
#define DLP_SRC_CORE_STATE_H_

#include <string>
#include <vector>
#include <memory>

#include "types.h"


namespace dlp {
namespace core {
class InstanceInfo;
class Atom;

/**
 * State stores information related to a planning state.
 */
class StateImpl {
private:
    const std::shared_ptr<const InstanceInfo> m_instance_info;
    // The underlying implementation currently is a vector of atoms.
    const Index_Vec m_atom_idxs;

public:
    StateImpl(std::shared_ptr<const InstanceInfo> instance_info, const std::vector<Atom>& atoms);
    StateImpl(std::shared_ptr<const InstanceInfo> instance_info, const Index_Vec& atom_idxs);
    StateImpl(std::shared_ptr<const InstanceInfo> instance_info, const Name_Vec& atom_names);
    ~StateImpl() = default;

    /**
     * Getters.
     */
    std::shared_ptr<const InstanceInfo> get_instance_info() const;
    const Index_Vec& get_atom_idxs() const;

    std::string str() const;

};

}
}

#endif
