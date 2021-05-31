#ifndef DLP_SRC_CORE_ELEMENT_FACTORY_H_
#define DLP_SRC_CORE_ELEMENT_FACTORY_H_

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "instance_info.h"
#include "cache.h"
#include "types.h"
#include "elements/types.h"


namespace dlp {
namespace core {

/**
 * ElementFactory provides functionality for constructing
 * nested composites of description logics elements
 * that can be evaluated on state of planning instances.
 */
class SyntacticElementFactoryImpl {
protected:
    ElementCache m_cache;

public:
    SyntacticElementFactoryImpl();


    element::ConceptElement_Ptr parse_concept_element(const std::string &description);
    element::RoleElement_Ptr parse_role_element(const std::string &description);
    element::NumericalElement_Ptr parse_numerical_element(const std::string &description);
    element::BooleanElement_Ptr parse_boolean_element(const std::string &description);

    element::BooleanElement_Ptr make_empty_boolean_element(element::ConceptElement_Ptr concept);
    element::BooleanElement_Ptr make_empty_boolean_element(element::RoleElement_Ptr role);

    element::ConceptElement_Ptr make_all_concept_element(element::RoleElement_Ptr role, element::ConceptElement_Ptr concept);
    element::ConceptElement_Ptr make_and_concept_element(element::ConceptElement_Ptr concept_left, element::ConceptElement_Ptr concept_right);
    element::ConceptElement_Ptr make_bot_concept_element();
    element::ConceptElement_Ptr make_diff_concept_element(element::ConceptElement_Ptr concept_left, element::ConceptElement_Ptr concept_right);
    element::ConceptElement_Ptr make_not_concept_element(element::ConceptElement_Ptr concept);
    element::ConceptElement_Ptr make_one_of_concept_element(unsigned pos);
    element::ConceptElement_Ptr make_or_concept_element(element::ConceptElement_Ptr concept_left, element::ConceptElement_Ptr concept_right);
    element::ConceptElement_Ptr make_primitive_concept_element(const std::string& name, unsigned pos);
    element::ConceptElement_Ptr make_some_concept_element(element::RoleElement_Ptr role, element::ConceptElement_Ptr concept);
    element::ConceptElement_Ptr make_subset_concept_element(element::RoleElement_Ptr role_left, element::RoleElement_Ptr role_right);
    element::ConceptElement_Ptr make_top_concept_element();

    element::NumericalElement_Ptr make_concept_distance_element(element::ConceptElement_Ptr concept_from, element::RoleElement_Ptr role, element::ConceptElement_Ptr concept_to);
    element::NumericalElement_Ptr make_count_element(element::ConceptElement_Ptr concept);
    element::NumericalElement_Ptr make_count_element(element::RoleElement_Ptr role);
    element::NumericalElement_Ptr make_role_distance_element(element::RoleElement_Ptr role_from, element::RoleElement_Ptr role, element::RoleElement_Ptr role_to);
    element::NumericalElement_Ptr make_sum_concept_distance_element(element::ConceptElement_Ptr concept_from, element::RoleElement_Ptr role, element::ConceptElement_Ptr concept_to);
    element::NumericalElement_Ptr make_sum_role_distance_element(element::RoleElement_Ptr role_from, element::RoleElement_Ptr role, element::RoleElement_Ptr role_to);

    element::RoleElement_Ptr make_and_role_element(element::RoleElement_Ptr role_left, element::RoleElement_Ptr role_right);
    element::RoleElement_Ptr make_compose_role_element(element::RoleElement_Ptr role_left, element::RoleElement_Ptr role_right);
    element::RoleElement_Ptr make_diff_role_element(element::RoleElement_Ptr role_left, element::RoleElement_Ptr role_right);
    element::RoleElement_Ptr make_identity_role_element(element::ConceptElement_Ptr concept);
    element::RoleElement_Ptr make_inverse_role_element(element::RoleElement_Ptr role);
    element::RoleElement_Ptr make_not_role_element(element::RoleElement_Ptr role);
    element::RoleElement_Ptr make_or_role_element(element::RoleElement_Ptr role_left, element::RoleElement_Ptr role_right);
    element::RoleElement_Ptr make_primitive_role_element(const std::string& name, unsigned pos_1, unsigned pos_2);
    element::RoleElement_Ptr make_restrict_role_element(element::RoleElement_Ptr role, element::ConceptElement_Ptr concept);
    element::RoleElement_Ptr make_top_role_element();
    element::RoleElement_Ptr make_transitive_closure_element(element::RoleElement_Ptr role);
    element::RoleElement_Ptr make_transitive_reflexive_closure_element(element::RoleElement_Ptr role);
};

}
}

#endif
