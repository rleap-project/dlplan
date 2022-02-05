#include "concept.h"


namespace dlplan::generator::rules {

std::function<std::pair<dlplan::core::Concept, std::array<uint32_t, 4>>(const States, const core::Concept&)> Concept::m_task =
[](const States& states, const core::Concept& element) {
    return std::make_pair(std::move(element),compute_hash(bitset_to_num_vec(evaluate<core::ConceptDenotation>(element, states))));
};

}
