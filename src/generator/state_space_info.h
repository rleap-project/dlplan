#ifndef DLP_SRC_CORE_STATE_SPACE_INFO_IMPL_H_
#define DLP_SRC_CORE_STATE_SPACE_INFO_IMPL_H_

#include "../../include/dlp/core.h"


namespace dlp {
namespace generator {

class StateSpaceInfoImpl {
private:
    std::vector<core::State> m_states;

public:
    StateSpaceInfoImpl() = default;
    ~StateSpaceInfoImpl() = default;

    int add_state(const core::State& state);
};

}
}


#endif
