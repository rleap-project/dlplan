#ifndef DLPLAN_SRC_CORE_ELEMENT_FACTORY_H_
#define DLPLAN_SRC_CORE_ELEMENT_FACTORY_H_

#include <memory>
#include <string>

#include "cache.h"
#include "types.h"

#include "../../include/dlplan/core.h"


namespace dlplan::core {
class VocabularyInfoImpl;


class SyntacticElementFactoryImpl {
private:
    std::shared_ptr<const VocabularyInfo> m_vocabulary_info;

    Caches m_caches;

public:
    SyntacticElementFactoryImpl(std::shared_ptr<const VocabularyInfo> vocabulary_info);

    Concept parse_concept(const std::string &description, int index=-1);
    Role parse_role(const std::string &description, int index=-1);
    Numerical parse_numerical(const std::string &description, int index=-1);
    Boolean parse_boolean(const std::string &description, int index=-1);

    Boolean make_empty_boolean(const Concept& concept, int index=-1);
    Boolean make_empty_boolean(const Role& role, int index=-1);
    Boolean make_concept_inclusion_boolean(const Concept& concept_left, const Concept& concept_right, int index=-1);
    Boolean make_role_inclusion_boolean(const Role& role_left, const Role& role_right, int index=-1);
    Boolean make_nullary_boolean(const Predicate& predicate, int index=-1);

    Concept make_all_concept(const Role& role, const Concept& concept, int index=-1);
    Concept make_and_concept(const Concept& concept_left, const Concept& concept_right, int index=-1);
    Concept make_bot_concept(int index=-1);
    Concept make_diff_concept(const Concept& concept_left, const Concept& concept_right, int index=-1);
    Concept make_equal_concept(const Role& role_left, const Role& role_right, int index=-1);
    Concept make_not_concept(const Concept& concept, int index=-1);
    Concept make_one_of_concept(const Constant& constant, int index=-1);
    Concept make_or_concept(const Concept& concept_left, const Concept& concept_right, int index=-1);
    Concept make_projection_concept(const Role& role, int pos, int index=-1);
    Concept make_primitive_concept(const Predicate& predicate, int pos, int index=-1);
    Concept make_some_concept(const Role& role, const Concept& concept, int index=-1);
    Concept make_subset_concept(const Role& role_left, const Role& role_right, int index=-1);
    Concept make_top_concept(int index);

    Numerical make_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to, int index=-1);
    Numerical make_count(const Concept& concept, int index=-1);
    Numerical make_count(const Role& role, int index=-1);
    Numerical make_role_distance(const Role& role_from, const Role& role, const Role& role_to, int index=-1);
    Numerical make_sum_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to, int index=-1);
    Numerical make_sum_role_distance(const Role& role_from, const Role& role, const Role& role_to, int index=-1);

    Role make_and_role(const Role& role_left, const Role& role_right, int index=-1);
    Role make_compose_role(const Role& role_left, const Role& role_right, int index=-1);
    Role make_diff_role(const Role& role_left, const Role& role_right, int index=-1);
    Role make_identity_role(const Concept& concept, int index=-1);
    Role make_inverse_role(const Role& role, int index=-1);
    Role make_not_role(const Role& role, int index=-1);
    Role make_or_role(const Role& role_left, const Role& role_right, int index=-1);
    Role make_primitive_role(const Predicate& predicate, int pos_1, int pos_2, int index=-1);
    Role make_restrict_role(const Role& role, const Concept& concept, int index=-1);
    Role make_top_role(int index=-1);
    Role make_transitive_closure(const Role& role, int index=-1);
    Role make_transitive_reflexive_closure(const Role& role, int index=-1);

    /**
     * Getters.
     */
    const VocabularyInfo& get_vocabulary_info_ref() const;
    std::shared_ptr<const VocabularyInfo> get_vocabulary_info() const;
};

}

#endif
