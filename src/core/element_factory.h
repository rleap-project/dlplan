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

    std::shared_ptr<const Concept> parse_concept(const std::string &description);
    std::shared_ptr<const Role> parse_role(const std::string &description);
    std::shared_ptr<const Numerical> parse_numerical(const std::string &description);
    std::shared_ptr<const Boolean> parse_boolean(const std::string &description);

    std::shared_ptr<const Boolean> make_empty_boolean(const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Boolean> make_empty_boolean(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Boolean> make_inclusion_boolean(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Boolean> make_inclusion_boolean(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Boolean> make_nullary_boolean(const Predicate& predicate);

    std::shared_ptr<const Concept> make_all_concept(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Concept> make_and_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Concept> make_bot_concept();
    std::shared_ptr<const Concept> make_diff_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Concept> make_equal_concept(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Concept> make_not_concept(const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Concept> make_one_of_concept(const Constant& constant);
    std::shared_ptr<const Concept> make_or_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Concept> make_projection_concept(const std::shared_ptr<const Role>& role, int pos);
    std::shared_ptr<const Concept> make_primitive_concept(const Predicate& predicate, int pos);
    std::shared_ptr<const Concept> make_some_concept(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Concept> make_subset_concept(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Concept> make_top_concept();

    std::shared_ptr<const Numerical> make_concept_distance_numerical(const std::shared_ptr<const Concept>& concept_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_to);
    std::shared_ptr<const Numerical> make_count_numerical(const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Numerical> make_count_numerical(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Numerical> make_role_distance_numerical(const std::shared_ptr<const Role>& role_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Role>& role_to);
    std::shared_ptr<const Numerical> make_sum_concept_distance_numerical(const std::shared_ptr<const Concept>& concept_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_to);
    std::shared_ptr<const Numerical> make_sum_role_distance_numerical(const std::shared_ptr<const Role>& role_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Role>& role_to);

    std::shared_ptr<const Role> make_and_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_compose_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_diff_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_identity_role(const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Role> make_inverse_role(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Role> make_not_role(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Role> make_or_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_primitive_role(const Predicate& predicate, int pos_1, int pos_2);
    std::shared_ptr<const Role> make_restrict_role(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Role> make_top_role();
    std::shared_ptr<const Role> make_transitive_closure(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Role> make_transitive_reflexive_closure(const std::shared_ptr<const Role>& role);

    /**
     * Getters.
     */
    std::shared_ptr<const VocabularyInfo> get_vocabulary_info() const;
};

}

#endif
