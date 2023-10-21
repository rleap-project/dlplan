#ifndef SRC_CORE_PARSERS_ELEMENTS_STAGE_1_PARSER_DEF_HPP_
#define SRC_CORE_PARSERS_ELEMENTS_STAGE_1_PARSER_DEF_HPP_

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#include "include/dlplan/core/parsers/elements/stage_1/error_handler.hpp"
#include "include/dlplan/core/parsers/elements/stage_1/ast.hpp"
#include "include/dlplan/core/parsers/elements/stage_1/parser.hpp"

#include "ast_adapted.hpp"
#include "parser.hpp"


namespace dlplan::core::parsers::elements::stage_1::parser
{
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    using x3::lit;
    using x3::lexeme;
    using x3::int_;
    using x3::eps;

    using ascii::alpha;
    using ascii::alnum;
    using ascii::char_;
    using ascii::string;


    ///////////////////////////////////////////////////////////////////////////
    // Rule IDs
    ///////////////////////////////////////////////////////////////////////////

    struct NameClass;
    struct ConstantClass;
    struct PredicateClass;
    struct PositionClass;
    struct BooleanInnerClass;
    struct ConceptInnerClass;
    struct NumericalInnerClass;
    struct RoleInnerClass;
    struct ElementInnerClass;
    struct ConceptOrRoleInnerClass;
    struct ConceptOrRoleClass;
    struct EmptyBooleanClass;
    struct InclusionBooleanClass;
    struct NullaryBooleanClass;
    struct AllConceptClass;
    struct AndConceptClass;
    struct BotConceptClass;
    struct DiffConceptClass;
    struct EqualConceptClass;
    struct NotConceptClass;
    struct OneOfConceptClass;
    struct OrConceptClass;
    struct PrimitiveConceptClass;
    struct ProjectionConceptClass;
    struct SomeConceptClass;
    struct SubsetConceptClass;
    struct TopConceptClass;
    struct ConceptDistanceNumericalClass;
    struct CountNumericalClass;
    struct RoleDistanceNumericalClass;
    struct SumConceptDistanceNumericalClass;
    struct SumRoleDistanceNumericalClass;
    struct AndRoleClass;
    struct ComposeRoleClass;
    struct DiffRoleClass;
    struct IdentityRoleClass;
    struct InverseRoleClass;
    struct NotRoleClass;
    struct OrRoleClass;
    struct PrimitiveRoleClass;
    struct RestrictRoleClass;
    struct TopRoleClass;
    struct TransitiveClosureRoleClass;
    struct TransitiveReflexiveClosureRoleClass;

    ///////////////////////////////////////////////////////////////////////////
    // Rules
    ///////////////////////////////////////////////////////////////////////////

    /* Private rules with annotation */
    x3::rule<NameClass, ast::Name> const
        name = "name";

    x3::rule<ConstantClass, ast::Constant> const
        constant = "constant";

    x3::rule<PredicateClass, ast::Predicate> const
        predicate = "predicate";

    x3::rule<PositionClass, ast::Position> const
        position = "position";

    x3::rule<BooleanInnerClass, ast::BooleanInner> const
        boolean_inner = "boolean_inner";

    x3::rule<ConceptInnerClass, ast::ConceptInner> const
        concept_inner = "concept_inner";

    x3::rule<NumericalInnerClass, ast::NumericalInner> const
        numerical_inner = "numerical_inner";

    x3::rule<RoleInnerClass, ast::RoleInner> const
        role_inner = "role_inner";

    x3::rule<ElementInnerClass, ast::ElementInner> const
        element_inner = "element_inner";

    x3::rule<ConceptOrRoleInnerClass, ast::ConceptOrRoleInner> const
        concept_or_role_inner = "concept_or_role_inner";

    x3::rule<ConceptOrRoleClass, ast::ConceptOrRole> const
        concept_or_role = "concept_or_role";

    x3::rule<EmptyBooleanClass, ast::EmptyBoolean> const
        empty_boolean = "empty_boolean";

    x3::rule<InclusionBooleanClass, ast::InclusionBoolean> const
        inclusion_boolean = "inclusion_boolean";

    x3::rule<NullaryBooleanClass, ast::NullaryBoolean> const
        nullary_boolean = "nullary_boolean";

    x3::rule<AllConceptClass, ast::AllConcept> const
        all_concept = "all_concept";

    x3::rule<AndConceptClass, ast::AndConcept> const
        and_concept = "and_concept";

    x3::rule<BotConceptClass, ast::BotConcept> const
        bot_concept = "bot_concept";

    x3::rule<DiffConceptClass, ast::DiffConcept> const
        diff_concept = "diff_concept";

    x3::rule<EqualConceptClass, ast::EqualConcept> const
        equal_concept = "equal_concept";

    x3::rule<NotConceptClass, ast::NotConcept> const
        not_concept = "not_concept";

    x3::rule<OneOfConceptClass, ast::OneOfConcept> const
        one_of_concept = "one_of_concept";

    x3::rule<OrConceptClass, ast::OrConcept> const
        or_concept = "or_concept";

    x3::rule<PrimitiveConceptClass, ast::PrimitiveConcept> const
        primitive_concept = "primitive_concept";

    x3::rule<ProjectionConceptClass, ast::ProjectionConcept> const
        projection_concept = "projection_concept";

    x3::rule<SomeConceptClass, ast::SomeConcept> const
        some_concept = "some_concept";

    x3::rule<SubsetConceptClass, ast::SubsetConcept> const
        subset_concept = "subset_concept";

    x3::rule<TopConceptClass, ast::TopConcept> const
        top_concept = "top_concept";

    x3::rule<ConceptDistanceNumericalClass, ast::ConceptDistanceNumerical> const
        concept_distance_numerical = "concept_distance_numerical";

    x3::rule<CountNumericalClass, ast::CountNumerical> const
        count_numerical = "count_numerical";

    x3::rule<RoleDistanceNumericalClass, ast::RoleDistanceNumerical> const
        role_distance_numerical = "role_distance_numerical";

    x3::rule<SumConceptDistanceNumericalClass, ast::SumConceptDistanceNumerical> const
        sum_concept_distance_numerical = "sum_concept_distance_numerical";

    x3::rule<SumRoleDistanceNumericalClass, ast::SumRoleDistanceNumerical> const
        sum_role_distance_numerical = "sum_role_distance_numerical";

    x3::rule<AndRoleClass, ast::AndRole> const
        and_role = "and_role";

    x3::rule<ComposeRoleClass, ast::ComposeRole> const
        compose_role = "compose_role";

    x3::rule<DiffRoleClass, ast::DiffRole> const
        diff_role = "diff_role";

    x3::rule<IdentityRoleClass, ast::IdentityRole> const
        identity_role = "identity_role";

    x3::rule<InverseRoleClass, ast::InverseRole> const
        inverse_role = "inverse_role";

    x3::rule<NotRoleClass, ast::NotRole> const
        not_role = "not_role";

    x3::rule<OrRoleClass, ast::OrRole> const
        or_role = "or_role";

    x3::rule<PrimitiveRoleClass, ast::PrimitiveRole> const
        primitive_role = "primitive_role";

    x3::rule<RestrictRoleClass, ast::RestrictRole> const
        restrict_role = "restrict_role";

    x3::rule<TopRoleClass, ast::TopRole> const
        top_role = "top_role";

    x3::rule<TransitiveClosureRoleClass, ast::TransitiveClosureRole> const
        transitive_closure_role = "transitive_closure_role";

    x3::rule<TransitiveReflexiveClosureRoleClass, ast::TransitiveReflexiveClosureRole> const
        transitive_reflexive_closure_role = "transitive_reflexive_closure_role";

    /* Privates rules with annotation and error handling */
    boolean_root_type const
        boolean_root = "boolean";

    numerical_root_type const
        numerical_root = "numerical";

    concept_root_type const
        concept_root = "concept";

    role_root_type const
        role_root = "role";

    element_root_type const
        element_root = "element";

    /* Public rules with annotation */
    boolean_type const
        boolean = "boolean";

    numerical_type const
        numerical = "numerical";

    concept_type const
        concept = "concept";

    role_type const role = "role";

    element_type const element = "element";


    ///////////////////////////////////////////////////////////////////////////
    // Grammar
    ///////////////////////////////////////////////////////////////////////////

    const auto name_def = alpha >> lexeme[*(alnum | char_('-') | char_('_'))];

    const auto constant_def = name;

    const auto predicate_def = name;

    const auto position_def = int_;

    const auto empty_boolean_def = lit("b_empty") > lit('(') > concept_or_role > lit(')');

    // could change this to two_concept_or_two_role to produce more verbose error
    const auto inclusion_boolean_def = lit("b_inclusion") > lit('(') > concept_or_role > lit(',') > concept_or_role > lit(')');

    const auto nullary_boolean_def = lit("b_nullary") > lit('(') > predicate > lit(')');

    const auto all_concept_def = lit("c_all") > lit('(') > role > lit(',') > concept > lit(')');

    const auto and_concept_def = lit("c_and") > lit('(') > concept > lit(',') > concept > lit(')');

    // Note: Need this semantic action to synthesize the empty struct
    const auto bot_concept_def = lit("c_bot") >> x3::attr(ast::BotConcept{});

    const auto diff_concept_def = lit("c_diff") > lit('(') > concept > lit(',') > concept > lit(')');

    const auto equal_concept_def = lit("c_equal") > lit('(') > role > lit(',') > role > lit(')');

    const auto not_concept_def = lit("c_not") > lit('(') > concept > lit(')');

    const auto one_of_concept_def = lit("c_one_of") > lit('(') > constant > lit(')');

    const auto or_concept_def = lit("c_or") > lit('(') > concept > lit(',') > concept > lit(')');

    const auto primitive_concept_def = lit("c_primitive") > lit('(') > predicate > lit(',') > position > lit(')');

    const auto projection_concept_def = lit("c_projection") > lit('(') > role > lit(',') > position > lit(')');

    const auto some_concept_def = lit("c_some") > lit('(') > role > lit(',') > concept > lit(')');

    const auto subset_concept_def = lit("c_subset") > lit('(') > role > lit(',') > role > lit(')');

    // Note: Need this semantic action to synthesize the empty struct
    const auto top_concept_def = lit("c_top") >> x3::attr(ast::TopConcept{});

    const auto concept_distance_numerical_def = lit("n_concept_distance") > lit('(') > concept > lit(',') > role > lit(',') > concept > lit(')');

    const auto count_numerical_def = lit("n_count") > lit('(') > concept_or_role > lit(')');

    const auto role_distance_numerical_def = lit("n_role_distance") > lit('(') > role > lit(',') > role > lit(',') > role > lit(')');

    const auto sum_concept_distance_numerical_def = lit("n_sum_concept_distance") > lit('(') > concept > lit(',') > role > lit(',') > concept > lit(')');

    const auto sum_role_distance_numerical_def = lit("n_sum_role_distance") > lit('(') > role > lit(',') > role > lit(',') > role > lit(')');

    const auto and_role_def = lit("r_and") > lit('(') > role > lit(',') > role > lit(')');

    const auto compose_role_def = lit("r_compose") > lit('(') > role > lit(',') > role > lit(')');

    const auto diff_role_def = lit("r_diff") > lit('(') > role > lit(',') > role > lit(')');

    const auto identity_role_def = lit("r_identity") > lit('(') > concept > lit(')');

    const auto inverse_role_def = lit("r_inverse") > lit('(') > role > lit(')');

    const auto not_role_def = lit("r_not") > lit('(') > role > lit(')');

    const auto or_role_def = lit("r_or") > lit('(') > role > lit(',') > role > lit(')');

    const auto primitive_role_def = lit("r_primitive") > lit('(') > predicate > lit(',') > position  > lit(',') > position > lit(')');

    const auto restrict_role_def = lit("r_restrict") > lit('(') > role > lit(',') > concept > lit(')');

    // Note: Need this semantic action to synthesize the empty struct
    const auto top_role_def = lit("r_top") >> x3::attr(ast::TopRole{});

    const auto transitive_closure_role_def = lit("r_transitive_closure") > lit('(') > role > lit(')');

    const auto transitive_reflexive_closure_role_def = lit("r_transitive_reflexive_closure") > lit('(') > role > lit(')');

    const auto boolean_inner_def = empty_boolean | inclusion_boolean | nullary_boolean;
    const auto boolean_def = boolean_inner;
    const auto boolean_root_def = boolean_inner;

    // Note: non recursive comes first, i.e., primitive_concept
    const auto concept_inner_def = primitive_concept | all_concept | and_concept | bot_concept | diff_concept | equal_concept | not_concept | one_of_concept | or_concept | projection_concept | some_concept | subset_concept | top_concept;
    const auto concept_def = concept_inner;
    const auto concept_root_def = concept_inner;

    const auto numerical_inner_def = concept_distance_numerical | count_numerical | role_distance_numerical | sum_concept_distance_numerical | sum_role_distance_numerical;
    const auto numerical_def = numerical_inner;
    const auto numerical_root_def = numerical_inner;

    // Note: non recursive comes first, i.e., primitive_role
    const auto role_inner_def = primitive_role | and_role | compose_role | diff_role | identity_role | inverse_role | not_role | or_role | restrict_role | top_role | transitive_closure_role | transitive_reflexive_closure_role;
    const auto role_def = role_inner;
    const auto role_root_def = role_inner;

    const auto concept_or_role_inner_def = concept_inner | role_inner;
    const auto concept_or_role_def = concept_or_role_inner;

    const auto element_inner_def = boolean_inner | concept_inner | numerical_inner | role_inner;
    const auto element_def = eps > element_inner;
    const auto element_root_def = eps > element_inner;


    BOOST_SPIRIT_DEFINE(
        name, constant, predicate, position,
        boolean_inner, boolean, boolean_root,
        concept_inner, concept, concept_root,
        numerical_inner, numerical, numerical_root,
        role_inner, role, role_root,
        element_inner, element, element_root,
        concept_or_role_inner, concept_or_role,
        empty_boolean, inclusion_boolean, nullary_boolean,
        all_concept, and_concept, bot_concept, diff_concept, equal_concept, not_concept, one_of_concept, or_concept, primitive_concept, projection_concept, some_concept, subset_concept, top_concept,
        concept_distance_numerical, count_numerical, role_distance_numerical, sum_concept_distance_numerical, sum_role_distance_numerical,
        and_role, compose_role, diff_role, identity_role, inverse_role, not_role, or_role, primitive_role, restrict_role, top_role, transitive_closure_role, transitive_reflexive_closure_role)

    ///////////////////////////////////////////////////////////////////////////
    // Annotation and Error handling
    ///////////////////////////////////////////////////////////////////////////

    struct NameClass : x3::annotate_on_success {};
    struct ConstantClass : x3::annotate_on_success {};
    struct PredicateClass : x3::annotate_on_success {};
    struct PositionClass : x3::annotate_on_success {};
    struct EmptyBooleanClass : x3::annotate_on_success {};
    struct InclusionBooleanClass : x3::annotate_on_success {};
    struct NullaryBooleanClass : x3::annotate_on_success {};
    struct AllConceptClass : x3::annotate_on_success {};
    struct AndConceptClass : x3::annotate_on_success {};
    struct BotConceptClass : x3::annotate_on_success {};
    struct DiffConceptClass : x3::annotate_on_success {};
    struct EqualConceptClass : x3::annotate_on_success {};
    struct NotConceptClass : x3::annotate_on_success {};
    struct OneOfConceptClass : x3::annotate_on_success {};
    struct OrConceptClass : x3::annotate_on_success {};
    struct PrimitiveConceptClass : x3::annotate_on_success {};
    struct ProjectionConceptClass : x3::annotate_on_success {};
    struct SomeConceptClass : x3::annotate_on_success {};
    struct SubsetConceptClass : x3::annotate_on_success {};
    struct TopConceptClass : x3::annotate_on_success {};
    struct ConceptDistanceNumericalClass : x3::annotate_on_success {};
    struct CountNumericalClass : x3::annotate_on_success {};
    struct RoleDistanceNumericalClass : x3::annotate_on_success {};
    struct SumConceptDistanceNumericalClass : x3::annotate_on_success {};
    struct SumRoleDistanceNumericalClass : x3::annotate_on_success {};
    struct AndRoleClass : x3::annotate_on_success {};
    struct ComposeRoleClass : x3::annotate_on_success {};
    struct DiffRoleClass : x3::annotate_on_success {};
    struct IdentityRoleClass : x3::annotate_on_success {};
    struct InverseRoleClass : x3::annotate_on_success {};
    struct NotRoleClass : x3::annotate_on_success {};
    struct OrRoleClass : x3::annotate_on_success {};
    struct PrimitiveRoleClass : x3::annotate_on_success {};
    struct RestrictRoleClass : x3::annotate_on_success {};
    struct TopRoleClass : x3::annotate_on_success {};
    struct TransitiveClosureRoleClass : x3::annotate_on_success {};
    struct TransitiveReflexiveClosureRoleClass : x3::annotate_on_success {};

    struct BooleanInnerClass : x3::annotate_on_success {};
    struct BooleanClass : x3::annotate_on_success {};
    struct BooleanRootClass : x3::annotate_on_success, error_handler_core {};

    struct NumericalInnerClass : x3::annotate_on_success {};
    struct NumericalClass : x3::annotate_on_success {};
    struct NumericalRootClass : x3::annotate_on_success, dlplan::common::parsers::error_handler_base {};

    struct ConceptInnerClass : x3::annotate_on_success {};
    struct ConceptClass : x3::annotate_on_success {};
    struct ConceptRootClass : x3::annotate_on_success, error_handler_core {};

    struct RoleInnerClass : x3::annotate_on_success {};
    struct RoleClass : x3::annotate_on_success {};
    struct RoleRootClass : x3::annotate_on_success, error_handler_core {};

    struct ElementInnerClass : x3::annotate_on_success {};
    struct ElementClass : x3::annotate_on_success {};
    struct ElementRootClass : x3::annotate_on_success, error_handler_core  {};

    struct ConceptOrRoleInnerClass : x3::annotate_on_success {};
    struct ConceptOrRoleClass : x3::annotate_on_success {};
}

namespace dlplan::core::parsers::elements::stage_1
{
    parser::element_type const& element()
    {
        return parser::element;
    }

    parser::boolean_type const& boolean()
    {
        return parser::boolean;
    }

    parser::numerical_type const& numerical()
    {
        return parser::numerical;
    }

    parser::concept_type const& concept()
    {
        return parser::concept;
    }

    parser::role_type const& role()
    {
        return parser::role;
    }
}


namespace dlplan::core::parsers::elements::stage_1
{
    parser::element_root_type const& element_root()
    {
        return parser::element_root;
    }

    parser::boolean_root_type const& boolean_root()
    {
        return parser::boolean_root;
    }

    parser::numerical_root_type const& numerical_root()
    {
        return parser::numerical_root;
    }

    parser::concept_root_type const& concept_root()
    {
        return parser::concept_root;
    }

    parser::role_root_type const& role_root()
    {
        return parser::role_root;
    }
}


#endif