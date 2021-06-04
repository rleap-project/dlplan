#include "state_space_info.h"


namespace dlp {
namespace generator {

int StateSpaceInfoImpl::add_state(const core::State& state) {
    m_states.push_back(state);
    return m_states.size() - 1;
}

}
}
