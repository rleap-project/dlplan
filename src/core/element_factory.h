#ifndef DLPLAN_SRC_CORE_ELEMENT_FACTORY_H_
#define DLPLAN_SRC_CORE_ELEMENT_FACTORY_H_

#include "cache.h"

#include "../../include/dlplan/core.h"

#include <memory>
#include <string>


namespace dlplan::core {
class SyntacticElementFactoryImpl {
private:
    std::shared_ptr<const VocabularyInfo> m_vocabulary_info;

    Caches m_caches;

public:
    SyntacticElementFactoryImpl(std::shared_ptr<const VocabularyInfo> vocabulary_info);

    Concept parse_concept(const std::string &description);
    Role parse_role(const std::string &description);
    Numerical parse_numerical(const std::string &description);
    Boolean parse_boolean(const std::string &description);

    Boolean make_empty_boolean(const Concept& concept);
    Boolean make_empty_boolean(const Role& role);
    Boolean make_inclusion_boolean(const Concept& concept_left, const Concept& concept_right);
    Boolean make_inclusion_boolean(const Role& role_left, const Role& role_right);
    Boolean make_nullary_boolean(const Predicate& predicate);

    Concept make_all_concept(const Role& role, const Concept& concept);
    Concept make_and_concept(const Concept& concept_left, const Concept& concept_right);
    Concept make_bot_concept();
    Concept make_diff_concept(const Concept& concept_left, const Concept& concept_right);
    Concept make_equal_concept(const Role& role_left, const Role& role_right);
    Concept make_not_concept(const Concept& concept);
    Concept make_one_of_concept(const Constant& constant);
    Concept make_or_concept(const Concept& concept_left, const Concept& concept_right);
    Concept make_projection_concept(const Role& role, int pos);
    Concept make_primitive_concept(const Predicate& predicate, int pos);
    Concept make_some_concept(const Role& role, const Concept& concept);
    Concept make_subset_concept(const Role& role_left, const Role& role_right);
    Concept make_top_concept();

    Numerical make_concept_distance_numerical(const Concept& concept_from, const Role& role, const Concept& concept_to);
    Numerical make_count_numerical(const Concept& concept);
    Numerical make_count_numerical(const Role& role);
    Numerical make_role_distance_numerical(const Role& role_from, const Role& role, const Role& role_to);
    Numerical make_sum_concept_distance_numerical(const Concept& concept_from, const Role& role, const Concept& concept_to);
    Numerical make_sum_role_distance_numerical(const Role& role_from, const Role& role, const Role& role_to);

    Role make_and_role(const Role& role_left, const Role& role_right);
    Role make_compose_role(const Role& role_left, const Role& role_right);
    Role make_diff_role(const Role& role_left, const Role& role_right);
    Role make_identity_role(const Concept& concept);
    Role make_inverse_role(const Role& role);
    Role make_not_role(const Role& role);
    Role make_or_role(const Role& role_left, const Role& role_right);
    Role make_primitive_role(const Predicate& predicate, int pos_1, int pos_2);
    Role make_restrict_role(const Role& role, const Concept& concept);
    Role make_top_role();
    Role make_transitive_closure(const Role& role);
    Role make_transitive_reflexive_closure(const Role& role);

    /**
     * Getters.
     */
    std::shared_ptr<const VocabularyInfo> get_vocabulary_info() const;
};

}

#endif
