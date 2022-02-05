#include "numerical.h"


namespace dlplan::generator::rules {

std::function<std::pair<dlplan::core::Numerical, std::array<uint32_t, 4>>(const States, const core::Numerical&)> Numerical::m_task =
[](const States& states, const core::Numerical& element) {
    return std::make_pair(element, compute_hash(evaluate<int>(element, states)));
};

}
