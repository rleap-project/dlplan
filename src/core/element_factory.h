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


    element::Concept_Ptr parse_concept_element(const std::string &description);
    element::Role_Ptr parse_role_element(const std::string &description);
    element::Numerical_Ptr parse_numerical_element(const std::string &description);
    element::Boolean_Ptr parse_boolean_element(const std::string &description);

    element::Boolean_Ptr make_empty_boolean_element(element::Concept_Ptr concept);
    element::Boolean_Ptr make_empty_boolean_element(element::Role_Ptr role);

    element::Concept_Ptr make_all_concept_element(element::Role_Ptr role, element::Concept_Ptr concept);
    element::Concept_Ptr make_and_concept_element(element::Concept_Ptr concept_left, element::Concept_Ptr concept_right);
    element::Concept_Ptr make_bot_concept_element();
    element::Concept_Ptr make_diff_concept_element(element::Concept_Ptr concept_left, element::Concept_Ptr concept_right);
    element::Concept_Ptr make_not_concept_element(element::Concept_Ptr concept);
    element::Concept_Ptr make_one_of_concept_element(unsigned pos);
    element::Concept_Ptr make_or_concept_element(element::Concept_Ptr concept_left, element::Concept_Ptr concept_right);
    element::Concept_Ptr make_primitive_concept_element(const std::string& name, unsigned pos);
    element::Concept_Ptr make_some_concept_element(element::Role_Ptr role, element::Concept_Ptr concept);
    element::Concept_Ptr make_subset_concept_element(element::Role_Ptr role_left, element::Role_Ptr role_right);
    element::Concept_Ptr make_top_concept_element();

    element::Numerical_Ptr make_concept_distance_element(element::Concept_Ptr concept_from, element::Role_Ptr role, element::Concept_Ptr concept_to);
    element::Numerical_Ptr make_count_element(element::Concept_Ptr concept);
    element::Numerical_Ptr make_count_element(element::Role_Ptr role);
    element::Numerical_Ptr make_role_distance_element(element::Role_Ptr role_from, element::Role_Ptr role, element::Role_Ptr role_to);
    element::Numerical_Ptr make_sum_concept_distance_element(element::Concept_Ptr concept_from, element::Role_Ptr role, element::Concept_Ptr concept_to);
    element::Numerical_Ptr make_sum_role_distance_element(element::Role_Ptr role_from, element::Role_Ptr role, element::Role_Ptr role_to);

    element::Role_Ptr make_and_role_element(element::Role_Ptr role_left, element::Role_Ptr role_right);
    element::Role_Ptr make_compose_role_element(element::Role_Ptr role_left, element::Role_Ptr role_right);
    element::Role_Ptr make_diff_role_element(element::Role_Ptr role_left, element::Role_Ptr role_right);
    element::Role_Ptr make_identity_role_element(element::Concept_Ptr concept);
    element::Role_Ptr make_inverse_role_element(element::Role_Ptr role);
    element::Role_Ptr make_not_role_element(element::Role_Ptr role);
    element::Role_Ptr make_or_role_element(element::Role_Ptr role_left, element::Role_Ptr role_right);
    element::Role_Ptr make_primitive_role_element(const std::string& name, unsigned pos_1, unsigned pos_2);
    element::Role_Ptr make_restrict_role_element(element::Role_Ptr role, element::Concept_Ptr concept);
    element::Role_Ptr make_top_role_element();
    element::Role_Ptr make_transitive_closure_element(element::Role_Ptr role);
    element::Role_Ptr make_transitive_reflexive_closure_element(element::Role_Ptr role);
};

}
}

#endif
