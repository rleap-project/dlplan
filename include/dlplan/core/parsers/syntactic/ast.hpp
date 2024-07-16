#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_PARSERS_SYNTACTIC_AST_HPP_
#define DLPLAN_INCLUDE_DLPLAN_CORE_PARSERS_SYNTACTIC_AST_HPP_

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include <vector>


namespace dlplan::core::ast
{
    ///////////////////////////////////////////////////////////////////////////
    //  The AST
    ///////////////////////////////////////////////////////////////////////////
    namespace x3 = boost::spirit::x3;

    struct Name;
    struct Constant;
    struct Predicate;
    struct Position;
    struct EmptyBoolean;
    struct InclusionBoolean;
    struct NullaryBoolean;
    struct AllConcept;
    struct AndConcept;
    struct BotConcept;
    struct DiffConcept;
    struct EqualConcept;
    struct NotConcept;
    struct OneOfConcept;
    struct OrConcept;
    struct PrimitiveConcept;
    struct ProjectionConcept;
    struct SomeConcept;
    struct SubsetConcept;
    struct TopConcept;
    struct ConceptDistanceNumerical;
    struct CountNumerical;
    struct RoleDistanceNumerical;
    struct SumConceptDistanceNumerical;
    struct SumRoleDistanceNumerical;
    struct AndRole;
    struct ComposeRole;
    struct DiffRole;
    struct IdentityRole;
    struct InverseRole;
    struct NotRole;
    struct OrRole;
    struct PrimitiveRole;
    struct RestrictRole;
    struct TilCRole;
    struct TopRole;
    struct TransitiveClosureRole;
    struct TransitiveReflexiveClosureRole;

    /* Basic character compounds */
    struct Name : x3::position_tagged {
        char alphabetical;
        std::string suffix;
    };

    struct Constant : x3::position_tagged {
        Name name;
    };

    struct Predicate : x3::position_tagged {
        Name name;
    };

    struct Integer : x3::position_tagged {
        int value;
    };

    struct Position : x3::position_tagged {
        Integer integer;
    };

    struct Boolean : x3::position_tagged, x3::variant<
        x3::forward_ast<EmptyBoolean>,
        x3::forward_ast<InclusionBoolean>,
        x3::forward_ast<NullaryBoolean>> {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct Concept : x3::position_tagged, x3::variant<
        x3::forward_ast<PrimitiveConcept>,
        x3::forward_ast<AllConcept>,
        x3::forward_ast<AndConcept>,
        x3::forward_ast<BotConcept>,
        x3::forward_ast<DiffConcept>,
        x3::forward_ast<EqualConcept>,
        x3::forward_ast<NotConcept>,
        x3::forward_ast<OneOfConcept>,
        x3::forward_ast<OrConcept>,
        x3::forward_ast<ProjectionConcept>,
        x3::forward_ast<SomeConcept>,
        x3::forward_ast<SubsetConcept>,
        x3::forward_ast<TopConcept>> {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct Numerical : x3::position_tagged, x3::variant<
        x3::forward_ast<ConceptDistanceNumerical>,
        x3::forward_ast<CountNumerical>,
        x3::forward_ast<RoleDistanceNumerical>,
        x3::forward_ast<SumConceptDistanceNumerical>,
        x3::forward_ast<SumRoleDistanceNumerical>> {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct Role : x3::position_tagged, x3::variant<
        x3::forward_ast<PrimitiveRole>,
        x3::forward_ast<AndRole>,
        x3::forward_ast<ComposeRole>,
        x3::forward_ast<DiffRole>,
        x3::forward_ast<IdentityRole>,
        x3::forward_ast<InverseRole>,
        x3::forward_ast<NotRole>,
        x3::forward_ast<OrRole>,
        x3::forward_ast<RestrictRole>,
        x3::forward_ast<TilCRole>,
        x3::forward_ast<TopRole>,
        x3::forward_ast<TransitiveClosureRole>,
        x3::forward_ast<TransitiveReflexiveClosureRole>> {
        using base_type::base_type;
        using base_type::operator=;
    };


    struct Element : x3::position_tagged, x3::variant<
        x3::forward_ast<Concept>,
        x3::forward_ast<Role>,
        x3::forward_ast<Boolean>,
        x3::forward_ast<Numerical>> {
        using base_type::base_type;
        using base_type::operator=;
    };


    struct ConceptOrRole : x3::position_tagged, x3::variant<
        x3::forward_ast<Concept>,
        x3::forward_ast<Role>> {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct EmptyBoolean : x3::position_tagged {
        ConceptOrRole element;
    };

    struct InclusionBoolean : x3::position_tagged {
        ConceptOrRole element_left;
        ConceptOrRole element_right;
    };

    struct NullaryBoolean : x3::position_tagged {
        Predicate predicate;
    };

    struct AllConcept : x3::position_tagged {
        Role role;
        Concept concept_;
    };

    struct AndConcept : x3::position_tagged {
        Concept concept_left;
        Concept concept_right;
    };

    struct BotConcept : x3::position_tagged {
    };

    struct DiffConcept : x3::position_tagged {
        Concept concept_left;
        Concept concept_right;
    };

    struct EqualConcept : x3::position_tagged {
        Role role_left;
        Role role_right;
    };

    struct NotConcept : x3::position_tagged {
        Concept concept_;
    };

    struct OneOfConcept : x3::position_tagged {
        Constant constant;
    };

    struct OrConcept : x3::position_tagged {
        Concept concept_left;
        Concept concept_right;
    };

    struct PrimitiveConcept : x3::position_tagged {
        Predicate predicate;
        Position pos;
    };

    struct ProjectionConcept : x3::position_tagged {
        Role role;
        Position pos;
    };

    struct SomeConcept : x3::position_tagged {
        Role role;
        Concept concept_;
    };

    struct SubsetConcept : x3::position_tagged {
        Role role_left;
        Role role_right;
    };

    struct TopConcept : x3::position_tagged {
    };

    struct ConceptDistanceNumerical : x3::position_tagged {
        Concept concept_left;
        Role role;
        Concept concept_right;
    };

    struct CountNumerical : x3::position_tagged {
        ConceptOrRole element;
    };

    struct RoleDistanceNumerical : x3::position_tagged {
        Role role_left;
        Role role;
        Role role_right;
    };

    struct SumConceptDistanceNumerical : x3::position_tagged {
        Concept concept_left;
        Role role;
        Concept concept_right;
    };

    struct SumRoleDistanceNumerical : x3::position_tagged {
        Role role_left;
        Role role;
        Role role_right;
    };

    struct AndRole : x3::position_tagged {
        Role role_left;
        Role role_right;
    };

    struct ComposeRole : x3::position_tagged {
        Role role_left;
        Role role_right;
    };

    struct DiffRole : x3::position_tagged {
        Role role_left;
        Role role_right;
    };

    struct IdentityRole : x3::position_tagged {
        Concept concept_;
    };

    struct InverseRole : x3::position_tagged {
        Role role;
    };

    struct NotRole : x3::position_tagged {
        Role role;
    };

    struct OrRole : x3::position_tagged {
        Role role_left;
        Role role_right;
    };

    struct PrimitiveRole : x3::position_tagged {
        Predicate predicate;
        Position pos_1;
        Position pos_2;
    };

    struct RestrictRole : x3::position_tagged {
        Role role;
        Concept concept_;
    };

    struct TilCRole : x3::position_tagged {
        Role role;
        Concept concept_;
    };

    struct TopRole : x3::position_tagged {
    };

    struct TransitiveClosureRole : x3::position_tagged {
        Role role;
    };

    struct TransitiveReflexiveClosureRole : x3::position_tagged {
        Role role;
    };
}


#endif