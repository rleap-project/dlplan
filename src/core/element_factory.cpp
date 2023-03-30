#include "element_factory.h"

#include "elements/booleans/empty.h"
#include "elements/booleans/inclusion.h"
#include "elements/booleans/nullary.h"
#include "elements/concepts/all.h"
#include "elements/concepts/bot.h"
#include "elements/concepts/and.h"
#include "elements/concepts/diff.h"
#include "elements/concepts/equal.h"
#include "elements/concepts/not.h"
#include "elements/concepts/one_of.h"
#include "elements/concepts/or.h"
#include "elements/concepts/projection.h"
#include "elements/concepts/primitive.h"
#include "elements/concepts/some.h"
#include "elements/concepts/subset.h"
#include "elements/concepts/top.h"
#include "elements/numericals/concept_distance.h"
#include "elements/numericals/count.h"
#include "elements/numericals/role_distance.h"
#include "elements/numericals/sum_concept_distance.h"
#include "elements/numericals/sum_role_distance.h"
#include "elements/roles/and.h"
#include "elements/roles/compose.h"
#include "elements/roles/diff.h"
#include "elements/roles/identity.h"
#include "elements/roles/inverse.h"
#include "elements/roles/not.h"
#include "elements/roles/or.h"
#include "elements/roles/primitive.h"
#include "elements/roles/restrict.h"
#include "elements/roles/top.h"
#include "elements/roles/transitive_closure.h"
#include "elements/roles/transitive_reflexive_closure.h"
#include "parser/parser.h"
#include "parser/expressions/expression.h"


namespace dlplan::core {
SyntacticElementFactoryImpl::SyntacticElementFactoryImpl(std::shared_ptr<const VocabularyInfo> vocabulary_info)
    : m_vocabulary_info(vocabulary_info), m_caches(Caches()) {
}

Concept SyntacticElementFactoryImpl::parse_concept(const std::string &description) {
    return Concept(m_vocabulary_info, parser::Parser().parse(description)->parse_concept(*m_vocabulary_info, m_caches));
}

Role SyntacticElementFactoryImpl::parse_role(const std::string &description) {
    return Role(m_vocabulary_info, parser::Parser().parse(description)->parse_role(*m_vocabulary_info, m_caches));
}

Numerical SyntacticElementFactoryImpl::parse_numerical(const std::string &description) {
    return Numerical(m_vocabulary_info, parser::Parser().parse(description)->parse_numerical(*m_vocabulary_info, m_caches));
}

Boolean SyntacticElementFactoryImpl::parse_boolean(const std::string &description) {
    return Boolean(m_vocabulary_info, parser::Parser().parse(description)->parse_boolean(*m_vocabulary_info, m_caches));
}

Boolean SyntacticElementFactoryImpl::make_empty_boolean(const Concept& concept) {
    return Boolean(m_vocabulary_info, m_caches.m_boolean_cache->insert(
        std::make_unique<element::EmptyBoolean<element::Concept>>(*m_vocabulary_info, concept.get_element())).first);
}

Boolean SyntacticElementFactoryImpl::make_empty_boolean(const Role& role) {
    return Boolean(m_vocabulary_info, m_caches.m_boolean_cache->insert(
        std::make_unique<element::EmptyBoolean<element::Role>>(*m_vocabulary_info, role.get_element())).first);
}

Boolean SyntacticElementFactoryImpl::make_inclusion_boolean(const Concept& concept_left, const Concept& concept_right) {
    return Boolean(m_vocabulary_info, m_caches.m_boolean_cache->insert(
        std::make_unique<element::InclusionBoolean<element::Concept>>(*m_vocabulary_info, concept_left.get_element(), concept_right.get_element())).first);
}

Boolean SyntacticElementFactoryImpl::make_inclusion_boolean(const Role& role_left, const Role& role_right) {
    return Boolean(m_vocabulary_info, m_caches.m_boolean_cache->insert(
        std::make_unique<element::InclusionBoolean<element::Role>>(*m_vocabulary_info, role_left.get_element(), role_right.get_element())).first);
}

Boolean SyntacticElementFactoryImpl::make_nullary_boolean(const Predicate& predicate) {
    return Boolean(m_vocabulary_info, m_caches.m_boolean_cache->insert(
        std::make_unique<element::NullaryBoolean>(*m_vocabulary_info, predicate)).first);
}

Concept SyntacticElementFactoryImpl::make_all_concept(const Role& role, const Concept& concept) {
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(
        std::make_unique<element::AllConcept>(*m_vocabulary_info, role.get_element(), concept.get_element())).first);
}

Concept SyntacticElementFactoryImpl::make_and_concept(const Concept& concept_left, const Concept& concept_right) {
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(
        std::make_unique<element::AndConcept>(*m_vocabulary_info, concept_left.get_element(), concept_right.get_element())).first);
}

Concept SyntacticElementFactoryImpl::make_bot_concept() {
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(
        std::make_unique<element::BotConcept>(*m_vocabulary_info)).first);
}

Concept SyntacticElementFactoryImpl::make_diff_concept(const Concept& concept_left, const Concept& concept_right) {
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(
        std::make_unique<element::DiffConcept>(*m_vocabulary_info, concept_left.get_element(), concept_right.get_element())).first);
}

Concept SyntacticElementFactoryImpl::make_equal_concept(const Role& role_left, const Role& role_right) {
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(
        std::make_unique<element::EqualConcept>(*m_vocabulary_info, role_left.get_element(), role_right.get_element())).first);
}

Concept SyntacticElementFactoryImpl::make_not_concept(const Concept& concept) {
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(std::make_unique<element::NotConcept>(*m_vocabulary_info, concept.get_element())).first);
}

Concept SyntacticElementFactoryImpl::make_one_of_concept(const Constant& constant) {
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(
        std::make_unique<element::OneOfConcept>(*m_vocabulary_info, constant)).first);
}

Concept SyntacticElementFactoryImpl::make_or_concept(const Concept& concept_left, const Concept& concept_right) {
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(std::make_unique<element::OrConcept>(*m_vocabulary_info, concept_left.get_element(), concept_right.get_element())).first);
}

Concept SyntacticElementFactoryImpl::make_projection_concept(const Role& role, int pos) {
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(
        std::make_unique<element::ProjectionConcept>(*m_vocabulary_info, role.get_element(), pos)).first);
}

Concept SyntacticElementFactoryImpl::make_primitive_concept(const Predicate& predicate, int pos) {
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(
        std::make_unique<element::PrimitiveConcept>(*m_vocabulary_info, predicate, pos)).first);
}

Concept SyntacticElementFactoryImpl::make_some_concept(const Role& role, const Concept& concept) {
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(
        std::make_unique<element::SomeConcept>(*m_vocabulary_info, role.get_element(), concept.get_element())).first);
}

Concept SyntacticElementFactoryImpl::make_subset_concept(const Role& role_left, const Role& role_right) {
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(
        std::make_unique<element::SubsetConcept>(*m_vocabulary_info, role_left.get_element(), role_right.get_element())).first);
}

Concept SyntacticElementFactoryImpl::make_top_concept() {
    return Concept(m_vocabulary_info, m_caches.m_concept_cache->insert(
        std::make_unique<element::TopConcept>(*m_vocabulary_info)).first);
}

Numerical SyntacticElementFactoryImpl::make_concept_distance_numerical(const Concept& concept_from, const Role& role, const Concept& concept_to) {
    return Numerical(m_vocabulary_info, m_caches.m_numerical_cache->insert(
        std::make_unique<element::ConceptDistanceNumerical>(*m_vocabulary_info, concept_from.get_element(), role.get_element(), concept_to.get_element())).first);
}

Numerical SyntacticElementFactoryImpl::make_count_numerical(const Concept& concept) {
    return Numerical(m_vocabulary_info, m_caches.m_numerical_cache->insert(
        std::make_unique<element::CountNumerical<element::Concept_Ptr>>(*m_vocabulary_info, concept.get_element())).first);
}

Numerical SyntacticElementFactoryImpl::make_count_numerical(const Role& role) {
    return Numerical(m_vocabulary_info, m_caches.m_numerical_cache->insert(
        std::make_unique<element::CountNumerical<element::Role_Ptr>>(*m_vocabulary_info, role.get_element())).first);
}

Numerical SyntacticElementFactoryImpl::make_role_distance_numerical(const Role& role_from, const Role& role, const Role& role_to) {
    return Numerical(m_vocabulary_info, m_caches.m_numerical_cache->insert(
        std::make_unique<element::RoleDistanceNumerical>(*m_vocabulary_info, role_from.get_element(), role.get_element(), role_to.get_element())).first);
}

Numerical SyntacticElementFactoryImpl::make_sum_concept_distance_numerical(const Concept& concept_from, const Role& role, const Concept& concept_to) {
    return Numerical(m_vocabulary_info, m_caches.m_numerical_cache->insert(
        std::make_unique<element::SumConceptDistanceNumerical>(*m_vocabulary_info, concept_from.get_element(), role.get_element(), concept_to.get_element())).first);
}

Numerical SyntacticElementFactoryImpl::make_sum_role_distance_numerical(const Role& role_from, const Role& role, const Role& role_to) {
    return Numerical(m_vocabulary_info, m_caches.m_numerical_cache->insert(
        std::make_unique<element::SumRoleDistanceNumerical>(*m_vocabulary_info, role_from.get_element(), role.get_element(), role_to.get_element())).first);
}

Role SyntacticElementFactoryImpl::make_and_role(const Role& role_left, const Role& role_right) {
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(
        std::make_unique<element::AndRole>(*m_vocabulary_info, role_left.get_element(), role_right.get_element())).first);
}

Role SyntacticElementFactoryImpl::make_compose_role(const Role& role_left, const Role& role_right) {
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(
        std::make_unique<element::ComposeRole>(*m_vocabulary_info, role_left.get_element(), role_right.get_element())).first);
}

Role SyntacticElementFactoryImpl::make_diff_role(const Role& role_left, const Role& role_right) {
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(
        std::make_unique<element::DiffRole>(*m_vocabulary_info, role_left.get_element(), role_right.get_element())).first);
}

Role SyntacticElementFactoryImpl::make_identity_role(const Concept& concept) {
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(
        std::make_unique<element::IdentityRole>(*m_vocabulary_info, concept.get_element())).first);
}

Role SyntacticElementFactoryImpl::make_inverse_role(const Role& role) {
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(
        std::make_unique<element::InverseRole>(*m_vocabulary_info, role.get_element())).first);
}

Role SyntacticElementFactoryImpl::make_not_role(const Role& role) {
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(
        std::make_unique<element::NotRole>(*m_vocabulary_info, role.get_element())).first);
}

Role SyntacticElementFactoryImpl::make_or_role(const Role& role_left, const Role& role_right) {
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(
        std::make_unique<element::OrRole>(*m_vocabulary_info, role_left.get_element(), role_right.get_element())).first);
}

Role SyntacticElementFactoryImpl::make_primitive_role(const Predicate& predicate, int pos_1, int pos_2) {
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(
        std::make_unique<element::PrimitiveRole>(*m_vocabulary_info, predicate, pos_1, pos_2)).first);
}

Role SyntacticElementFactoryImpl::make_restrict_role(const Role& role, const Concept& concept) {
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(
        std::make_unique<element::RestrictRole>(*m_vocabulary_info, role.get_element(), concept.get_element())).first);
}

Role SyntacticElementFactoryImpl::make_top_role() {
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(
        std::make_unique<element::TopRole>(*m_vocabulary_info)).first);
}

Role SyntacticElementFactoryImpl::make_transitive_closure(const Role& role) {
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(
        std::make_unique<element::TransitiveClosureRole>(*m_vocabulary_info, role.get_element())).first);
}

Role SyntacticElementFactoryImpl::make_transitive_reflexive_closure(const Role& role) {
    return Role(m_vocabulary_info, m_caches.m_role_cache->insert(
        std::make_unique<element::TransitiveReflexiveClosureRole>(*m_vocabulary_info, role.get_element())).first);
}

std::shared_ptr<const VocabularyInfo> SyntacticElementFactoryImpl::get_vocabulary_info() const {
    return m_vocabulary_info;
}

}
