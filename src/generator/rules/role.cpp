#include "role.h"


namespace dlplan::generator::rules {

std::function<std::pair<dlplan::core::Role, std::array<uint32_t, 4>>(const States, const core::Role&)> Role::m_task =
[](const States& states, const core::Role& element) {
    return std::make_pair(std::move(element),compute_hash(bitset_to_num_vec(evaluate<core::RoleDenotation>(element, states))));
};

}
