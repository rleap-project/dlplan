#include "concept_distance.h"

namespace dlplan::generator::rules {

std::function<NumericalTaskResult(const States&, const core::Concept&, const core::Role&, const core::Numerical&)> ConceptDistanceNumerical::m_concept_distance_task =
[](const States& states, const core::Concept& concept, const core::Role& role, const core::Numerical& element) {
    auto concept_denotations = evaluate_concept(concept, states);
    // check if R is a restriction in R:C
    if (role.compute_complexity() == 3 && role.compute_repr().substr(0, 10) != "r_restrict") {
        return NumericalTaskResult(
            core::Numerical(element),
            -1,
            "",
            std::array<uint32_t, 4>(),
            true);
    }
    // check if left concept always evaluates to single object
    for (const auto& state : states) {
        if (concept.evaluate(state).size() != 1) {
            return NumericalTaskResult(
                core::Numerical(element),
                -1,
                "",
                std::array<uint32_t, 4>(),
                true);
        }
    }
    return NumericalTaskResult(
        core::Numerical(element),
        element.compute_complexity(),
        element.compute_repr(),
        compute_hash(evaluate_numerical(element, states)),
        false);
};

}