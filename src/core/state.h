#ifndef DLP_SRC_CORE_STATE_H_
#define DLP_SRC_CORE_STATE_H_

#include <string>
#include <vector>

#include "types.h"

namespace dlp {
namespace core {

/**
 * State stores information related to a planning state.
 */
struct StateImpl {
    // The underlying implementation currently is a vector of atoms.
    const Index_Vec m_atoms;

    StateImpl(const Index_Vec& atoms);
    StateImpl(Index_Vec&& atoms);
    ~StateImpl() = default;
};

}
}

#endif
