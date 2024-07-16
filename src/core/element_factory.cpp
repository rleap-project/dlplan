#include "element_factory.h"

#include "parsers/syntactic/parser.hpp"
#include "../../include/dlplan/common/parsers/config.hpp"
#include "../../include/dlplan/core/parsers/semantic/parser.hpp"
#include "../../include/dlplan/common/parsers/utility.hpp"

#include <cassert>
#include <string>

using namespace dlplan;


namespace dlplan::core {

SyntacticElementFactoryImpl::SyntacticElementFactoryImpl(std::shared_ptr<VocabularyInfo> vocabulary_info)
    : m_vocabulary_info(vocabulary_info) {
}

std::shared_ptr<const Concept> SyntacticElementFactoryImpl::parse_concept(SyntacticElementFactory& parent,
    const std::string &description, const std::string& filename) {
    iterator_type iter(description.begin());
    iterator_type const end(description.end());
    return parse_concept(parent, iter, end, filename);
}

std::shared_ptr<const Concept> SyntacticElementFactoryImpl::parse_concept(SyntacticElementFactory& parent,
    iterator_type& iter, iterator_type end, const std::string& filename) {
    /* Stage 1 parse */
    // Our parser
    using boost::spirit::x3::with;

    // Our error handler
    error_handler_type error_handler(iter, end, std::cerr, filename);
    auto const parser =
        // we pass our error handler to the parser so we can access
        // it later on in our on_error and on_sucess handlers
        with<error_handler_tag>(std::ref(error_handler)) [
            concept_root()
        ];

    // Our AST
    ast::Concept ast;

    // Go forth and parse!
    using boost::spirit::x3::ascii::space;
    bool success = phrase_parse(iter, end, parser, space, ast);
    if (!success) {
        throw std::runtime_error("Failed parse.");
    }

    /* Stage 2 parse */
    auto feature = parse(ast, error_handler, parent);

    return feature;
}

std::shared_ptr<const Role> SyntacticElementFactoryImpl::parse_role(SyntacticElementFactory& parent,
    const std::string &description, const std::string& filename) {
    iterator_type iter(description.begin());
    iterator_type const end(description.end());
    return parse_role(parent, iter, end, filename);
}

std::shared_ptr<const Role> SyntacticElementFactoryImpl::parse_role(SyntacticElementFactory& parent,
    iterator_type& iter, iterator_type end, const std::string& filename) {
    /* Stage 1 parse */
    // Our parser
    using boost::spirit::x3::with;

    // Our error handler
    error_handler_type error_handler(iter, end, std::cerr, filename);
    auto const parser =
        // we pass our error handler to the parser so we can access
        // it later on in our on_error and on_sucess handlers
        with<error_handler_tag>(std::ref(error_handler)) [
            role_root()
        ];

    // Our AST
    ast::Role ast;

    // Go forth and parse!
    using boost::spirit::x3::ascii::space;
    bool success = phrase_parse(iter, end, parser, space, ast);
    if (!success) {
        throw std::runtime_error("Failed parse.");
    }

    /* Stage 2 parse */
    auto feature = parse(ast, error_handler, parent);

    return feature;
}

std::shared_ptr<const Boolean> SyntacticElementFactoryImpl::parse_boolean(SyntacticElementFactory& parent,
    const std::string &description, const std::string& filename) {
    iterator_type iter(description.begin());
    iterator_type const end(description.end());
    return parse_boolean(parent, iter, end, filename);
}

std::shared_ptr<const Boolean> SyntacticElementFactoryImpl::parse_boolean(SyntacticElementFactory& parent,
    iterator_type& iter, iterator_type end, const std::string& filename) {
    /* Stage 1 parse */
    // Our parser
    using boost::spirit::x3::with;

    // Our error handler
    error_handler_type error_handler(iter, end, std::cerr, filename);
    auto const parser =
        // we pass our error handler to the parser so we can access
        // it later on in our on_error and on_sucess handlers
        with<error_handler_tag>(std::ref(error_handler)) [
            boolean_root()
        ];

    // Our AST
    ast::Boolean ast;

    // Go forth and parse!
    using boost::spirit::x3::ascii::space;
    bool success = phrase_parse(iter, end, parser, space, ast);
    if (!success) {
        throw std::runtime_error("Failed parse.");
    }

    /* Stage 2 parse */
    auto feature = parse(ast, error_handler, parent);

    return feature;
}

std::shared_ptr<const Numerical> SyntacticElementFactoryImpl::parse_numerical(SyntacticElementFactory& parent,
    const std::string &description, const std::string& filename) {
    iterator_type iter(description.begin());
    iterator_type const end(description.end());
    return parse_numerical(parent, iter, end, filename);
}

std::shared_ptr<const Numerical> SyntacticElementFactoryImpl::parse_numerical(SyntacticElementFactory& parent,
    iterator_type& iter, iterator_type end, const std::string& filename) {
    /* Stage 1 parse */
    // Our parser
    using boost::spirit::x3::with;

    // Our error handler
    error_handler_type error_handler(iter, end, std::cerr, filename);
    auto const parser =
        // we pass our error handler to the parser so we can access
        // it later on in our on_error and on_sucess handlers
        with<error_handler_tag>(std::ref(error_handler)) [
            numerical_root()
        ];

    // Our AST
    ast::Numerical ast;

    // Go forth and parse!
    using boost::spirit::x3::ascii::space;
    bool success = phrase_parse(iter, end, parser, space, ast);
    if (!success) {
        throw std::runtime_error("Failed parse.");
    }

    /* Stage 2 parse */
    auto feature = parse(ast, error_handler, parent);

    return feature;
}

std::shared_ptr<const Boolean> SyntacticElementFactoryImpl::make_empty_boolean(const std::shared_ptr<const Concept>& concept_) {
    return m_cache.get_or_create<EmptyBoolean<Concept>>(m_vocabulary_info, concept_).object;
}

std::shared_ptr<const Boolean> SyntacticElementFactoryImpl::make_empty_boolean(const std::shared_ptr<const Role>& role) {
    return m_cache.get_or_create<EmptyBoolean<Role>>(m_vocabulary_info, role).object;
}

std::shared_ptr<const Boolean> SyntacticElementFactoryImpl::make_inclusion_boolean(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right) {
    return m_cache.get_or_create<InclusionBoolean<Concept>>(m_vocabulary_info, concept_left, concept_right).object;
}

std::shared_ptr<const Boolean> SyntacticElementFactoryImpl::make_inclusion_boolean(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right) {
    return m_cache.get_or_create<InclusionBoolean<Role>>(m_vocabulary_info, role_left, role_right).object;
}

std::shared_ptr<const Boolean> SyntacticElementFactoryImpl::make_nullary_boolean(const Predicate& predicate) {
    return m_cache.get_or_create<NullaryBoolean>(m_vocabulary_info, predicate).object;
}

std::shared_ptr<const Concept> SyntacticElementFactoryImpl::make_all_concept(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_) {
    return m_cache.get_or_create<AllConcept>(m_vocabulary_info, role, concept_).object;
}

std::shared_ptr<const Concept> SyntacticElementFactoryImpl::make_and_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right) {
    return m_cache.get_or_create<AndConcept>(m_vocabulary_info, concept_left, concept_right).object;
}

std::shared_ptr<const Concept> SyntacticElementFactoryImpl::make_bot_concept() {
    return m_cache.get_or_create<BotConcept>(m_vocabulary_info).object;
}

std::shared_ptr<const Concept> SyntacticElementFactoryImpl::make_diff_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right) {
    return m_cache.get_or_create<DiffConcept>(m_vocabulary_info, concept_left, concept_right).object;
}

std::shared_ptr<const Concept> SyntacticElementFactoryImpl::make_equal_concept(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right) {
    return m_cache.get_or_create<EqualConcept>(m_vocabulary_info, role_left, role_right).object;
}

std::shared_ptr<const Concept> SyntacticElementFactoryImpl::make_not_concept(const std::shared_ptr<const Concept>& concept_) {
    return m_cache.get_or_create<NotConcept>(m_vocabulary_info, concept_).object;
}

std::shared_ptr<const Concept> SyntacticElementFactoryImpl::make_one_of_concept(const Constant& constant) {
    return m_cache.get_or_create<OneOfConcept>(m_vocabulary_info, constant).object;
}

std::shared_ptr<const Concept> SyntacticElementFactoryImpl::make_or_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right) {
    return m_cache.get_or_create<OrConcept>(m_vocabulary_info, concept_left, concept_right).object;
}

std::shared_ptr<const Concept> SyntacticElementFactoryImpl::make_projection_concept(const std::shared_ptr<const Role>& role, int pos) {
    return m_cache.get_or_create<ProjectionConcept>(m_vocabulary_info, role, pos).object;
}

std::shared_ptr<const Concept> SyntacticElementFactoryImpl::make_primitive_concept(const Predicate& predicate, int pos) {
    return m_cache.get_or_create<PrimitiveConcept>(m_vocabulary_info, predicate, pos).object;
}

std::shared_ptr<const Concept> SyntacticElementFactoryImpl::make_some_concept(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_) {
    return m_cache.get_or_create<SomeConcept>(m_vocabulary_info, role, concept_).object;
}

std::shared_ptr<const Concept> SyntacticElementFactoryImpl::make_subset_concept(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right) {
    return m_cache.get_or_create<SubsetConcept>(m_vocabulary_info, role_left, role_right).object;
}

std::shared_ptr<const Concept> SyntacticElementFactoryImpl::make_top_concept() {
    return m_cache.get_or_create<TopConcept>(m_vocabulary_info).object;
}

std::shared_ptr<const Numerical> SyntacticElementFactoryImpl::make_concept_distance_numerical(const std::shared_ptr<const Concept>& concept_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_to) {
    return m_cache.get_or_create<ConceptDistanceNumerical>(m_vocabulary_info, concept_from, role, concept_to).object;
}

std::shared_ptr<const Numerical> SyntacticElementFactoryImpl::make_count_numerical(const std::shared_ptr<const Concept>& concept_) {
    return m_cache.get_or_create<CountNumerical<Concept>>(m_vocabulary_info, concept_).object;
}

std::shared_ptr<const Numerical> SyntacticElementFactoryImpl::make_count_numerical(const std::shared_ptr<const Role>& role) {
    return m_cache.get_or_create<CountNumerical<Role>>(m_vocabulary_info,role).object;
}

std::shared_ptr<const Numerical> SyntacticElementFactoryImpl::make_role_distance_numerical(const std::shared_ptr<const Role>& role_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Role>& role_to) {
    return m_cache.get_or_create<RoleDistanceNumerical>(m_vocabulary_info, role_from, role, role_to).object;
}

std::shared_ptr<const Numerical> SyntacticElementFactoryImpl::make_sum_concept_distance_numerical(const std::shared_ptr<const Concept>& concept_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_to) {
    return m_cache.get_or_create<SumConceptDistanceNumerical>(m_vocabulary_info, concept_from, role, concept_to).object;
}

std::shared_ptr<const Numerical> SyntacticElementFactoryImpl::make_sum_role_distance_numerical(const std::shared_ptr<const Role>& role_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Role>& role_to) {
    return m_cache.get_or_create<SumRoleDistanceNumerical>(m_vocabulary_info, role_from, role, role_to).object;
}

std::shared_ptr<const Role> SyntacticElementFactoryImpl::make_and_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right) {
    return m_cache.get_or_create<AndRole>(m_vocabulary_info, role_left, role_right).object;
}

std::shared_ptr<const Role> SyntacticElementFactoryImpl::make_compose_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right) {
    return m_cache.get_or_create<ComposeRole>(m_vocabulary_info, role_left, role_right).object;
}

std::shared_ptr<const Role> SyntacticElementFactoryImpl::make_diff_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right) {
    return m_cache.get_or_create<DiffRole>(m_vocabulary_info, role_left, role_right).object;
}

std::shared_ptr<const Role> SyntacticElementFactoryImpl::make_identity_role(const std::shared_ptr<const Concept>& concept_) {
    return m_cache.get_or_create<IdentityRole>(m_vocabulary_info, concept_).object;
}

std::shared_ptr<const Role> SyntacticElementFactoryImpl::make_inverse_role(const std::shared_ptr<const Role>& role) {
    return m_cache.get_or_create<InverseRole>(m_vocabulary_info, role).object;
}

std::shared_ptr<const Role> SyntacticElementFactoryImpl::make_not_role(const std::shared_ptr<const Role>& role) {
    return m_cache.get_or_create<NotRole>(m_vocabulary_info, role).object;
}

std::shared_ptr<const Role> SyntacticElementFactoryImpl::make_or_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right) {
    return m_cache.get_or_create<OrRole>(m_vocabulary_info, role_left, role_right).object;
}

std::shared_ptr<const Role> SyntacticElementFactoryImpl::make_primitive_role(const Predicate& predicate, int pos_1, int pos_2) {
    return m_cache.get_or_create<PrimitiveRole>(m_vocabulary_info, predicate, pos_1, pos_2).object;
}

std::shared_ptr<const Role> SyntacticElementFactoryImpl::make_restrict_role(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_) {
    return m_cache.get_or_create<RestrictRole>(m_vocabulary_info, role, concept_).object;
}

std::shared_ptr<const Role> SyntacticElementFactoryImpl::make_til_c_role(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_) {
    return m_cache.get_or_create<TilCRole>(m_vocabulary_info, role, concept_).object;
}

std::shared_ptr<const Role> SyntacticElementFactoryImpl::make_top_role() {
    return m_cache.get_or_create<TopRole>(m_vocabulary_info).object;
}

std::shared_ptr<const Role> SyntacticElementFactoryImpl::make_transitive_closure(const std::shared_ptr<const Role>& role) {
    return m_cache.get_or_create<TransitiveClosureRole>(m_vocabulary_info, role).object;
}

std::shared_ptr<const Role> SyntacticElementFactoryImpl::make_transitive_reflexive_closure(const std::shared_ptr<const Role>& role) {
    return m_cache.get_or_create<TransitiveReflexiveClosureRole>(m_vocabulary_info, role).object;
}

std::shared_ptr<VocabularyInfo> SyntacticElementFactoryImpl::get_vocabulary_info() const {
    return m_vocabulary_info;
}

}

namespace dlplan {
    // explicit template instantiations
    template class ReferenceCountedObjectFactory<core::EmptyBoolean<core::Concept>
        , core::EmptyBoolean<core::Role>
        , core::InclusionBoolean<core::Concept>
        , core::InclusionBoolean<core::Role>
        , core::NullaryBoolean
        , core::AllConcept
        , core::AndConcept
        , core::BotConcept
        , core::DiffConcept
        , core::EqualConcept
        , core::NotConcept
        , core::OneOfConcept
        , core::OrConcept
        , core::PrimitiveConcept
        , core::ProjectionConcept
        , core::SomeConcept
        , core::SubsetConcept
        , core::TopConcept
        , core::ConceptDistanceNumerical
        , core::CountNumerical<core::Concept>
        , core::CountNumerical<core::Role>
        , core::RoleDistanceNumerical
        , core::SumConceptDistanceNumerical
        , core::SumRoleDistanceNumerical
        , core::AndRole
        , core::ComposeRole
        , core::DiffRole
        , core::IdentityRole
        , core::InverseRole
        , core::NotRole
        , core::OrRole
        , core::PrimitiveRole
        , core::RestrictRole
        , core::TilCRole
        , core::TopRole
        , core::TransitiveClosureRole
        , core::TransitiveReflexiveClosureRole>;
}
