#ifndef DLP_SRC_CORE_STATE_H_
#define DLP_SRC_CORE_STATE_H_

#include <string>
#include <vector>
#include <memory>

#include "types.h"


namespace dlp {
namespace core {
class InstanceInfoImpl;

/**
 * State stores information related to a planning state.
 */
struct StateImpl {
    const std::shared_ptr<const InstanceInfoImpl> m_instance_info;
    // The underlying implementation currently is a vector of atoms.
    const Index_Vec m_atom_idxs;

    StateImpl(const InstanceInfoImpl& parent, const Index_Vec& atom_idxs);
    StateImpl(const InstanceInfoImpl& parent, Index_Vec&& atom_idxs);
    ~StateImpl() = default;

    /**
     * Getters.
     */
    const std::shared_ptr<const InstanceInfoImpl> get_instance_info() const;
    const Index_Vec& get_atom_idxs() const;
};

}
}

#endif
