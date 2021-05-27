#ifndef DLP_SRC_CORE_STATE_H_
#define DLP_SRC_CORE_STATE_H_

#include <string>
#include <vector>

#include "types.h"

namespace dlp {
namespace core {
class InstanceInfoImpl;

/**
 * State stores information related to a planning state.
 */
struct StateImpl {
    // Non-owning parent pointer
    const InstanceInfoImpl* m_parent;
    // The underlying implementation currently is a vector of atoms.
    const Index_Vec m_atoms;

    StateImpl(const InstanceInfoImpl* parent, const Index_Vec& atoms);
    StateImpl(const InstanceInfoImpl* parent, Index_Vec&& atoms);
    ~StateImpl() = default;
};

}
}

#endif
