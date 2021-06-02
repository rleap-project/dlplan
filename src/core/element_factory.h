#ifndef DLP_SRC_CORE_ELEMENT_FACTORY_H_
#define DLP_SRC_CORE_ELEMENT_FACTORY_H_

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "vocabulary_info.h"
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
    const std::shared_ptr<const VocabularyInfoImpl> m_vocabulary_info;

    ElementCache m_cache;

public:
    SyntacticElementFactoryImpl(const VocabularyInfoImpl& vocabulary_info);

    element::Concept_Ptr parse_concept(const std::string &description);
    element::Role_Ptr parse_role(const std::string &description);
    element::Numerical_Ptr parse_numerical(const std::string &description);
    element::Boolean_Ptr parse_boolean(const std::string &description);

    element::Boolean_Ptr make_empty_boolean(element::Concept_Ptr concept);
    element::Boolean_Ptr make_empty_boolean(element::Role_Ptr role);

    element::Concept_Ptr make_all_concept(element::Role_Ptr role, element::Concept_Ptr concept);
    element::Concept_Ptr make_and_concept(element::Concept_Ptr concept_left, element::Concept_Ptr concept_right);
    element::Concept_Ptr make_bot_concept();
    element::Concept_Ptr make_diff_concept(element::Concept_Ptr concept_left, element::Concept_Ptr concept_right);
    element::Concept_Ptr make_not_concept(element::Concept_Ptr concept);
    element::Concept_Ptr make_one_of_concept(const std::string& object_name);
    element::Concept_Ptr make_or_concept(element::Concept_Ptr concept_left, element::Concept_Ptr concept_right);
    element::Concept_Ptr make_primitive_concept(const std::string& name, unsigned pos);
    element::Concept_Ptr make_some_concept(element::Role_Ptr role, element::Concept_Ptr concept);
    element::Concept_Ptr make_subset_concept(element::Role_Ptr role_left, element::Role_Ptr role_right);
    element::Concept_Ptr make_top_concept();

    element::Numerical_Ptr make_concept_distance(element::Concept_Ptr concept_from, element::Role_Ptr role, element::Concept_Ptr concept_to);
    element::Numerical_Ptr make_count(element::Concept_Ptr concept);
    element::Numerical_Ptr make_count(element::Role_Ptr role);
    element::Numerical_Ptr make_role_distance(element::Role_Ptr role_from, element::Role_Ptr role, element::Role_Ptr role_to);
    element::Numerical_Ptr make_sum_concept_distance(element::Concept_Ptr concept_from, element::Role_Ptr role, element::Concept_Ptr concept_to);
    element::Numerical_Ptr make_sum_role_distance(element::Role_Ptr role_from, element::Role_Ptr role, element::Role_Ptr role_to);

    element::Role_Ptr make_and_role(element::Role_Ptr role_left, element::Role_Ptr role_right);
    element::Role_Ptr make_compose_role(element::Role_Ptr role_left, element::Role_Ptr role_right);
    element::Role_Ptr make_diff_role(element::Role_Ptr role_left, element::Role_Ptr role_right);
    element::Role_Ptr make_identity_role(element::Concept_Ptr concept);
    element::Role_Ptr make_inverse_role(element::Role_Ptr role);
    element::Role_Ptr make_not_role(element::Role_Ptr role);
    element::Role_Ptr make_or_role(element::Role_Ptr role_left, element::Role_Ptr role_right);
    element::Role_Ptr make_primitive_role(const std::string& name, unsigned pos_1, unsigned pos_2);
    element::Role_Ptr make_restrict_role(element::Role_Ptr role, element::Concept_Ptr concept);
    element::Role_Ptr make_top_role();
    element::Role_Ptr make_transitive_closure(element::Role_Ptr role);
    element::Role_Ptr make_transitive_reflexive_closure(element::Role_Ptr role);
};

}
}

#endif
