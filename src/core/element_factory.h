#ifndef DLP_SRC_CORE_ELEMENT_FACTORY_H_
#define DLP_SRC_CORE_ELEMENT_FACTORY_H_

#include <memory>
#include <string>

#include "cache.h"
#include "types.h"

#include "../../include/dlp/core.h"


namespace dlp {
namespace core {
class VocabularyInfoImpl;

/**
 * ElementFactory provides functionality for constructing
 * nested composites of description logics elements
 * that can be evaluated on state of planning instances.
 */
class SyntacticElementFactoryImpl {
protected:
    const std::shared_ptr<const VocabularyInfo> m_vocabulary_info;

    ElementCache m_cache;

public:
    SyntacticElementFactoryImpl(std::shared_ptr<const VocabularyInfo> vocabulary_info);

    Concept parse_concept(const std::string &description);
    Role parse_role(const std::string &description);
    Numerical parse_numerical(const std::string &description);
    Boolean parse_boolean(const std::string &description);

    Boolean make_empty_boolean(const Concept& concept);
    Boolean make_empty_boolean(const Role& role);
    // TODO(dominik): Do we want to pass Object/Predicate instead of strings?
    Concept make_all_concept(const Role& role, const Concept& concept);
    Concept make_and_concept(const Concept& concept_left, const Concept& concept_right);
    Concept make_bot_concept();
    Concept make_diff_concept(const Concept& concept_left, const Concept& concept_right);
    Concept make_not_concept(const Concept& concept);
    Concept make_one_of_concept(const std::string& object_name);
    Concept make_or_concept(const Concept& concept_left, const Concept& concept_right);
    Concept make_primitive_concept(const std::string& name, int pos);
    Concept make_some_concept(const Role& role, const Concept& concept);
    Concept make_subset_concept(const Role& role_left, const Role& role_right);
    Concept make_top_concept();

    Numerical make_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to);
    Numerical make_count(const Concept& concept);
    Numerical make_count(const Role& role);
    Numerical make_role_distance(const Role& role_from, const Role& role, const Role& role_to);
    Numerical make_sum_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to);
    Numerical make_sum_role_distance(const Role& role_from, const Role& role, const Role& role_to);

    Role make_and_role(const Role& role_left, const Role& role_right);
    Role make_compose_role(const Role& role_left, const Role& role_right);
    Role make_diff_role(const Role& role_left, const Role& role_right);
    Role make_identity_role(const Concept& concept);
    Role make_inverse_role(const Role& role);
    Role make_not_role(const Role& role);
    Role make_or_role(const Role& role_left, const Role& role_right);
    Role make_primitive_role(const std::string& name, int pos_1, int pos_2);
    Role make_restrict_role(const Role& role, const Concept& concept);
    Role make_top_role();
    Role make_transitive_closure(const Role& role);
    Role make_transitive_reflexive_closure(const Role& role);

    /**
     * Getters.
     */
    const VocabularyInfo* get_vocabulary_info() const;
};

}
}

#endif
