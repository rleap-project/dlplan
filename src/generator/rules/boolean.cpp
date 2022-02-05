#include "boolean.h"


namespace dlplan::generator::rules {

std::function<std::pair<dlplan::core::Boolean, std::array<uint32_t, 4>>(const States, const core::Boolean&)> Boolean::m_task =
[](const States& states, const core::Boolean& element) {
    return std::make_pair(element, compute_hash(bool_vec_to_num_vec(evaluate<bool>(element, states))));
};

}
