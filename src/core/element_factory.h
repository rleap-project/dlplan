#ifndef DLPLAN_SRC_CORE_ELEMENT_FACTORY_H_
#define DLPLAN_SRC_CORE_ELEMENT_FACTORY_H_

#include "../../include/dlplan/core/elements/booleans/empty.h"
#include "../../include/dlplan/core/elements/booleans/inclusion.h"
#include "../../include/dlplan/core/elements/booleans/nullary.h"
#include "../../include/dlplan/core/elements/concepts/all.h"
#include "../../include/dlplan/core/elements/concepts/and.h"
#include "../../include/dlplan/core/elements/concepts/bot.h"
#include "../../include/dlplan/core/elements/concepts/diff.h"
#include "../../include/dlplan/core/elements/concepts/equal.h"
#include "../../include/dlplan/core/elements/concepts/not.h"
#include "../../include/dlplan/core/elements/concepts/one_of.h"
#include "../../include/dlplan/core/elements/concepts/or.h"
#include "../../include/dlplan/core/elements/concepts/projection.h"
#include "../../include/dlplan/core/elements/concepts/primitive.h"
#include "../../include/dlplan/core/elements/concepts/some.h"
#include "../../include/dlplan/core/elements/concepts/subset.h"
#include "../../include/dlplan/core/elements/concepts/top.h"
#include "../../include/dlplan/core/elements/numericals/concept_distance.h"
#include "../../include/dlplan/core/elements/numericals/count.h"
#include "../../include/dlplan/core/elements/numericals/role_distance.h"
#include "../../include/dlplan/core/elements/numericals/sum_concept_distance.h"
#include "../../include/dlplan/core/elements/numericals/sum_role_distance.h"
#include "../../include/dlplan/core/elements/roles/and.h"
#include "../../include/dlplan/core/elements/roles/compose.h"
#include "../../include/dlplan/core/elements/roles/diff.h"
#include "../../include/dlplan/core/elements/roles/identity.h"
#include "../../include/dlplan/core/elements/roles/inverse.h"
#include "../../include/dlplan/core/elements/roles/not.h"
#include "../../include/dlplan/core/elements/roles/or.h"
#include "../../include/dlplan/core/elements/roles/primitive.h"
#include "../../include/dlplan/core/elements/roles/restrict.h"
#include "../../include/dlplan/core/elements/roles/til_c.h"
#include "../../include/dlplan/core/elements/roles/top.h"
#include "../../include/dlplan/core/elements/roles/transitive_closure.h"
#include "../../include/dlplan/core/elements/roles/transitive_reflexive_closure.h"

#include "../../include/dlplan/core.h"
#include "../../include/dlplan/utils/factory.h"

#include <memory>
#include <string>


namespace dlplan::core {
class SyntacticElementFactoryImpl {
private:
    std::shared_ptr<VocabularyInfo> m_vocabulary_info;

    ReferenceCountedObjectFactory<EmptyBoolean<Concept>
        , EmptyBoolean<Role>
        , InclusionBoolean<Concept>
        , InclusionBoolean<Role>
        , NullaryBoolean
        , AllConcept
        , AndConcept
        , BotConcept
        , DiffConcept
        , EqualConcept
        , NotConcept
        , OneOfConcept
        , OrConcept
        , PrimitiveConcept
        , ProjectionConcept
        , SomeConcept
        , SubsetConcept
        , TopConcept
        , ConceptDistanceNumerical
        , CountNumerical<Concept>
        , CountNumerical<Role>
        , RoleDistanceNumerical
        , SumConceptDistanceNumerical
        , SumRoleDistanceNumerical
        , AndRole
        , ComposeRole
        , DiffRole
        , IdentityRole
        , InverseRole
        , NotRole
        , OrRole
        , PrimitiveRole
        , RestrictRole
        , TilCRole
        , TopRole
        , TransitiveClosureRole
        , TransitiveReflexiveClosureRole> m_cache;

public:
    SyntacticElementFactoryImpl(std::shared_ptr<VocabularyInfo> vocabulary_info);

    // Root calls
    std::shared_ptr<const Concept> parse_concept(SyntacticElementFactory& parent,
        const std::string &description, const std::string& filename);
    std::shared_ptr<const Role> parse_role(SyntacticElementFactory& parent,
        const std::string &description, const std::string& filename);
    std::shared_ptr<const Numerical> parse_numerical(SyntacticElementFactory& parent,
        const std::string &description, const std::string& filename);
    std::shared_ptr<const Boolean> parse_boolean(SyntacticElementFactory& parent,
        const std::string &description, const std::string& filename);

    // Nested calls
    std::shared_ptr<const Concept> parse_concept(SyntacticElementFactory& parent,
        iterator_type& iter, iterator_type end, const std::string& filename);
    std::shared_ptr<const Role> parse_role(SyntacticElementFactory& parent,
        iterator_type& iter, iterator_type end, const std::string& filename);
    std::shared_ptr<const Numerical> parse_numerical(SyntacticElementFactory& parent,
        iterator_type& iter, iterator_type end, const std::string& filename);
    std::shared_ptr<const Boolean> parse_boolean(SyntacticElementFactory& parent,
        iterator_type& iter, iterator_type end, const std::string& filename);

    std::shared_ptr<const Boolean> make_empty_boolean(const std::shared_ptr<const Concept>& concept_);
    std::shared_ptr<const Boolean> make_empty_boolean(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Boolean> make_inclusion_boolean(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Boolean> make_inclusion_boolean(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Boolean> make_nullary_boolean(const Predicate& predicate);

    std::shared_ptr<const Concept> make_all_concept(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_);
    std::shared_ptr<const Concept> make_and_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Concept> make_bot_concept();
    std::shared_ptr<const Concept> make_diff_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Concept> make_equal_concept(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Concept> make_not_concept(const std::shared_ptr<const Concept>& concept_);
    std::shared_ptr<const Concept> make_one_of_concept(const Constant& constant);
    std::shared_ptr<const Concept> make_or_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Concept> make_projection_concept(const std::shared_ptr<const Role>& role, int pos);
    std::shared_ptr<const Concept> make_primitive_concept(const Predicate& predicate, int pos);
    std::shared_ptr<const Concept> make_some_concept(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_);
    std::shared_ptr<const Concept> make_subset_concept(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Concept> make_top_concept();

    std::shared_ptr<const Numerical> make_concept_distance_numerical(const std::shared_ptr<const Concept>& concept_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_to);
    std::shared_ptr<const Numerical> make_count_numerical(const std::shared_ptr<const Concept>& concept_);
    std::shared_ptr<const Numerical> make_count_numerical(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Numerical> make_role_distance_numerical(const std::shared_ptr<const Role>& role_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Role>& role_to);
    std::shared_ptr<const Numerical> make_sum_concept_distance_numerical(const std::shared_ptr<const Concept>& concept_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_to);
    std::shared_ptr<const Numerical> make_sum_role_distance_numerical(const std::shared_ptr<const Role>& role_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Role>& role_to);

    std::shared_ptr<const Role> make_and_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_compose_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_diff_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_identity_role(const std::shared_ptr<const Concept>& concept_);
    std::shared_ptr<const Role> make_inverse_role(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Role> make_not_role(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Role> make_or_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_primitive_role(const Predicate& predicate, int pos_1, int pos_2);
    std::shared_ptr<const Role> make_restrict_role(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_);
    std::shared_ptr<const Role> make_til_c_role(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_);
    std::shared_ptr<const Role> make_top_role();
    std::shared_ptr<const Role> make_transitive_closure(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Role> make_transitive_reflexive_closure(const std::shared_ptr<const Role>& role);

    /**
     * Getters.
     */
    std::shared_ptr<VocabularyInfo> get_vocabulary_info() const;
};

}

#endif
