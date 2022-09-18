#include "equal.h"

#include <cassert>

#include "../../../core/elements/roles/primitive.h"

namespace dlplan::generator::rules {

std::function<ConceptTaskResult(const States&, const core::Role&, const core::Concept&, core::element::GeneratorEvaluationCaches&)> EqualConcept::m_equal_concept_task =
    [](const States& states, const core::Role& role, const core::Concept& element, core::element::GeneratorEvaluationCaches& caches) {
    auto primitive_role = std::dynamic_pointer_cast<const core::element::PrimitiveRole>(role.get_element());
    assert(primitive_role);
    std::string predicate_name = primitive_role->get_predicate_ref().get_name();
    if (predicate_name.substr(predicate_name.size() - 2, 2) != "_g") {
        return ConceptTaskResult(
            core::Concept(element),
            -1,
            "",
            std::array<uint32_t, 4>(),
            true);
    }
    return ConceptTaskResult(
        core::Concept(element),
        element.compute_complexity(),
        element.compute_repr(),
        compute_hash(evaluate_concept(*element.get_element(), states, caches)),
        false);
};

}
