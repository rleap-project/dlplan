#include "element_factory.h"
#include "vocabulary_info.h"
#include "parser/parser.h"
#include "parser/expressions/expression.h"
#include "elements/booleans/empty.h"
#include "elements/concepts/all.h"
#include "elements/concepts/bot.h"
#include "elements/concepts/and.h"
#include "elements/concepts/diff.h"
#include "elements/concepts/not.h"
#include "elements/concepts/one_of.h"
#include "elements/concepts/or.h"
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
#include "concept.h"
#include "role.h"
#include "numerical.h"
#include "boolean.h"


namespace dlp {
namespace core {

SyntacticElementFactoryImpl::SyntacticElementFactoryImpl(const VocabularyInfoImpl& vocabulary_info)
    : m_vocabulary_info(vocabulary_info.shared_from_this()) {
}

Concept SyntacticElementFactoryImpl::parse_concept(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(*m_vocabulary_info, description);
    element::Concept_Ptr result_ptr = expression->parse_concept(*m_vocabulary_info, m_cache);
    return Concept(std::move(ConceptImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Role SyntacticElementFactoryImpl::parse_role(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(*m_vocabulary_info, description);
    element::Role_Ptr result_ptr = expression->parse_role(*m_vocabulary_info, m_cache);
    return Role(std::move(RoleImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Numerical SyntacticElementFactoryImpl::parse_numerical(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(*m_vocabulary_info, description);
    element::Numerical_Ptr result_ptr = expression->parse_numerical(*m_vocabulary_info, m_cache);
    return Numerical(std::move(NumericalImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Boolean SyntacticElementFactoryImpl::parse_boolean(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(*m_vocabulary_info, description);
    element::Boolean_Ptr result_ptr = expression->parse_boolean(*m_vocabulary_info, m_cache);
    return Boolean(std::move(BooleanImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Boolean SyntacticElementFactoryImpl::make_empty_boolean(const Concept& concept) {
    element::Concept_Ptr concept_ptr = m_cache.concept_element_cache().at(concept.compute_repr());
    element::Boolean_Ptr result_ptr = std::make_shared<element::EmptyBoolean<element::Concept_Ptr>>(*m_vocabulary_info, concept_ptr);
    result_ptr = m_cache.boolean_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Boolean(std::move(BooleanImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Boolean SyntacticElementFactoryImpl::make_empty_boolean(const Role& role) {
    element::Role_Ptr role_ptr = m_cache.role_element_cache().at(role.compute_repr());
    element::Boolean_Ptr result_ptr = std::make_shared<element::EmptyBoolean<element::Role_Ptr>>(*m_vocabulary_info, role_ptr);
    result_ptr = m_cache.boolean_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Boolean(std::move(BooleanImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Concept SyntacticElementFactoryImpl::make_all_concept(const Role& role, const Concept& concept) {
    element::Role_Ptr role_ptr = m_cache.role_element_cache().at(role.compute_repr());
    element::Concept_Ptr concept_ptr = m_cache.concept_element_cache().at(concept.compute_repr());
    element::Concept_Ptr result_ptr = std::make_shared<element::AllConcept>(*m_vocabulary_info, role_ptr, concept_ptr);
    result_ptr = m_cache.concept_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Concept(std::move(ConceptImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Concept SyntacticElementFactoryImpl::make_and_concept(const Concept& concept_left, const Concept& concept_right) {
    element::Concept_Ptr concept_left_ptr = m_cache.concept_element_cache().at(concept_left.compute_repr());
    element::Concept_Ptr concept_right_ptr = m_cache.concept_element_cache().at(concept_right.compute_repr());
    element::Concept_Ptr result_ptr = std::make_shared<element::AndConcept>(*m_vocabulary_info, concept_left_ptr, concept_right_ptr);
    result_ptr = m_cache.concept_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Concept(std::move(ConceptImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Concept SyntacticElementFactoryImpl::make_bot_concept() {
    element::Concept_Ptr result_ptr = std::make_shared<element::BotConcept>(*m_vocabulary_info);
    result_ptr = m_cache.concept_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Concept(std::move(ConceptImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Concept SyntacticElementFactoryImpl::make_diff_concept(const Concept& concept_left, const Concept& concept_right) {
    element::Concept_Ptr concept_left_ptr = m_cache.concept_element_cache().at(concept_left.compute_repr());
    element::Concept_Ptr concept_right_ptr = m_cache.concept_element_cache().at(concept_right.compute_repr());
    element::Concept_Ptr result_ptr = std::make_shared<element::DiffConcept>(*m_vocabulary_info, concept_left_ptr, concept_right_ptr);
    result_ptr = m_cache.concept_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Concept(std::move(ConceptImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Concept SyntacticElementFactoryImpl::make_not_concept(const Concept& concept) {
    element::Concept_Ptr concept_ptr = m_cache.concept_element_cache().at(concept.compute_repr());
    element::Concept_Ptr result_ptr = std::make_shared<element::NotConcept>(*m_vocabulary_info, concept_ptr);
    result_ptr = m_cache.concept_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Concept(std::move(ConceptImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Concept SyntacticElementFactoryImpl::make_one_of_concept(const std::string& object_name) {
    element::Concept_Ptr result_ptr = std::make_shared<element::OneOfConcept>(*m_vocabulary_info, object_name);
    result_ptr = m_cache.concept_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Concept(std::move(ConceptImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Concept SyntacticElementFactoryImpl::make_or_concept(const Concept& concept_left, const Concept& concept_right) {
    element::Concept_Ptr concept_left_ptr = m_cache.concept_element_cache().at(concept_left.compute_repr());
    element::Concept_Ptr concept_right_ptr = m_cache.concept_element_cache().at(concept_right.compute_repr());
    element::Concept_Ptr result_ptr = std::make_shared<element::OrConcept>(*m_vocabulary_info, concept_left_ptr, concept_right_ptr);
    result_ptr = m_cache.concept_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Concept(std::move(ConceptImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Concept SyntacticElementFactoryImpl::make_primitive_concept(const std::string& name, unsigned pos) {
    element::Concept_Ptr result_ptr = std::make_shared<element::PrimitiveConcept>(*m_vocabulary_info, name, pos);
    result_ptr = m_cache.concept_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Concept(std::move(ConceptImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Concept SyntacticElementFactoryImpl::make_some_concept(const Role& role, const Concept& concept) {
    element::Role_Ptr role_ptr = m_cache.role_element_cache().at(role.compute_repr());
    element::Concept_Ptr concept_ptr = m_cache.concept_element_cache().at(concept.compute_repr());
    element::Concept_Ptr result_ptr = std::make_shared<element::SomeConcept>(*m_vocabulary_info, role_ptr, concept_ptr);
    result_ptr = m_cache.concept_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Concept(std::move(ConceptImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Concept SyntacticElementFactoryImpl::make_subset_concept(const Role& role_left, const Role& role_right) {
    element::Role_Ptr role_left_ptr = m_cache.role_element_cache().at(role_left.compute_repr());
    element::Role_Ptr role_right_ptr = m_cache.role_element_cache().at(role_right.compute_repr());
    element::Concept_Ptr result_ptr = std::make_shared<element::SubsetConcept>(*m_vocabulary_info, role_left_ptr, role_right_ptr);
    result_ptr = m_cache.concept_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Concept(std::move(ConceptImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Concept SyntacticElementFactoryImpl::make_top_concept() {
    element::Concept_Ptr result_ptr = std::make_shared<element::TopConcept>(*m_vocabulary_info);
    result_ptr = m_cache.concept_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Concept(std::move(ConceptImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Numerical SyntacticElementFactoryImpl::make_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to) {
    element::Concept_Ptr concept_from_ptr = m_cache.concept_element_cache().at(concept_from.compute_repr());
    element::Role_Ptr role_ptr = m_cache.role_element_cache().at(role.compute_repr());
    element::Concept_Ptr concept_to_ptr = m_cache.concept_element_cache().at(concept_to.compute_repr());
    element::Numerical_Ptr result_ptr = std::make_shared<element::ConceptDistanceNumerical>(*m_vocabulary_info, concept_from_ptr, role_ptr, concept_to_ptr);
    result_ptr = m_cache.numerical_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Numerical(std::move(NumericalImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Numerical SyntacticElementFactoryImpl::make_count(const Concept& concept) {
    element::Concept_Ptr concept_ptr = m_cache.concept_element_cache().at(concept.compute_repr());
    element::Numerical_Ptr result_ptr = std::make_shared<element::CountNumerical<element::Concept_Ptr>>(*m_vocabulary_info, concept_ptr);
    result_ptr = m_cache.numerical_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Numerical(std::move(NumericalImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Numerical SyntacticElementFactoryImpl::make_count(const Role& role) {
    element::Role_Ptr role_ptr = m_cache.role_element_cache().at(role.compute_repr());
    element::Numerical_Ptr result_ptr = std::make_shared<element::CountNumerical<element::Role_Ptr>>(*m_vocabulary_info, role_ptr);
    result_ptr = m_cache.numerical_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Numerical(std::move(NumericalImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Numerical SyntacticElementFactoryImpl::make_role_distance(const Role& role_from, const Role& role, const Role& role_to) {
    element::Role_Ptr role_from_ptr = m_cache.role_element_cache().at(role_from.compute_repr());
    element::Role_Ptr role_ptr = m_cache.role_element_cache().at(role.compute_repr());
    element::Role_Ptr role_to_ptr = m_cache.role_element_cache().at(role_to.compute_repr());
    element::Numerical_Ptr result_ptr = std::make_shared<element::RoleDistanceNumerical>(*m_vocabulary_info, role_from_ptr, role_ptr, role_to_ptr);
    result_ptr = m_cache.numerical_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Numerical(std::move(NumericalImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Numerical SyntacticElementFactoryImpl::make_sum_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to) {
    element::Concept_Ptr concept_from_ptr = m_cache.concept_element_cache().at(concept_from.compute_repr());
    element::Role_Ptr role_ptr = m_cache.role_element_cache().at(role.compute_repr());
    element::Concept_Ptr concept_to_ptr = m_cache.concept_element_cache().at(concept_to.compute_repr());
    element::Numerical_Ptr result_ptr = std::make_shared<element::SumConceptDistanceNumerical>(*m_vocabulary_info, concept_from_ptr, role_ptr, concept_to_ptr);
    result_ptr = m_cache.numerical_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Numerical(std::move(NumericalImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Numerical SyntacticElementFactoryImpl::make_sum_role_distance(const Role& role_from, const Role& role, const Role& role_to) {
    element::Role_Ptr role_from_ptr = m_cache.role_element_cache().at(role_from.compute_repr());
    element::Role_Ptr role_ptr = m_cache.role_element_cache().at(role.compute_repr());
    element::Role_Ptr role_to_ptr = m_cache.role_element_cache().at(role_to.compute_repr());
    element::Numerical_Ptr result_ptr = std::make_shared<element::SumRoleDistanceNumerical>(*m_vocabulary_info, role_from_ptr, role_ptr, role_to_ptr);
    result_ptr = m_cache.numerical_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Numerical(std::move(NumericalImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Role SyntacticElementFactoryImpl::make_and_role(const Role& role_left, const Role& role_right) {
    element::Role_Ptr role_left_ptr = m_cache.role_element_cache().at(role_left.compute_repr());
    element::Role_Ptr role_right_ptr = m_cache.role_element_cache().at(role_right.compute_repr());
    element::Role_Ptr result_ptr = std::make_shared<element::AndRole>(*m_vocabulary_info, role_left_ptr, role_right_ptr);
    result_ptr = m_cache.role_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Role(std::move(RoleImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Role SyntacticElementFactoryImpl::make_compose_role(const Role& role_left, const Role& role_right) {
    element::Role_Ptr role_left_ptr = m_cache.role_element_cache().at(role_left.compute_repr());
    element::Role_Ptr role_right_ptr = m_cache.role_element_cache().at(role_right.compute_repr());
    element::Role_Ptr result_ptr = std::make_shared<element::ComposeRole>(*m_vocabulary_info, role_left_ptr, role_right_ptr);
    result_ptr = m_cache.role_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Role(std::move(RoleImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Role SyntacticElementFactoryImpl::make_diff_role(const Role& role_left, const Role& role_right) {
    element::Role_Ptr role_left_ptr = m_cache.role_element_cache().at(role_left.compute_repr());
    element::Role_Ptr role_right_ptr = m_cache.role_element_cache().at(role_right.compute_repr());
    element::Role_Ptr result_ptr = std::make_shared<element::DiffRole>(*m_vocabulary_info, role_left_ptr, role_right_ptr);
    result_ptr = m_cache.role_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Role(std::move(RoleImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Role SyntacticElementFactoryImpl::make_identity_role(const Concept& concept) {
    element::Concept_Ptr concept_ptr = m_cache.concept_element_cache().at(concept.compute_repr());
    element::Role_Ptr result_ptr = std::make_shared<element::IdentityRole>(*m_vocabulary_info, concept_ptr);
    result_ptr = m_cache.role_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Role(std::move(RoleImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Role SyntacticElementFactoryImpl::make_inverse_role(const Role& role) {
    element::Role_Ptr role_ptr = m_cache.role_element_cache().at(role.compute_repr());
    element::Role_Ptr result_ptr = std::make_shared<element::InverseRole>(*m_vocabulary_info, role_ptr);
    result_ptr = m_cache.role_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Role(std::move(RoleImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Role SyntacticElementFactoryImpl::make_not_role(const Role& role) {
    element::Role_Ptr role_ptr = m_cache.role_element_cache().at(role.compute_repr());
    element::Role_Ptr result_ptr = std::make_shared<element::NotRole>(*m_vocabulary_info, role_ptr);
    result_ptr = m_cache.role_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Role(std::move(RoleImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Role SyntacticElementFactoryImpl::make_or_role(const Role& role_left, const Role& role_right) {
    element::Role_Ptr role_left_ptr = m_cache.role_element_cache().at(role_left.compute_repr());
    element::Role_Ptr role_right_ptr = m_cache.role_element_cache().at(role_right.compute_repr());
    element::Role_Ptr result_ptr = std::make_shared<element::OrRole>(*m_vocabulary_info, role_left_ptr, role_right_ptr);
    result_ptr = m_cache.role_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Role(std::move(RoleImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Role SyntacticElementFactoryImpl::make_primitive_role(const std::string& name, unsigned pos_1, unsigned pos_2) {
    element::Role_Ptr result_ptr = std::make_shared<element::PrimitiveRole>(*m_vocabulary_info, name, pos_1, pos_2);
    result_ptr = m_cache.role_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Role(std::move(RoleImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Role SyntacticElementFactoryImpl::make_restrict_role(const Role& role, const Concept& concept) {
    element::Role_Ptr role_ptr = m_cache.role_element_cache().at(role.compute_repr());
    element::Concept_Ptr concept_ptr = m_cache.concept_element_cache().at(concept.compute_repr());
    element::Role_Ptr result_ptr = std::make_shared<element::RestrictRole>(*m_vocabulary_info, role_ptr, concept_ptr);
    result_ptr = m_cache.role_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Role(std::move(RoleImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Role SyntacticElementFactoryImpl::make_top_role() {
    element::Role_Ptr result_ptr = std::make_shared<element::TopRole>(*m_vocabulary_info);
    result_ptr = m_cache.role_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Role(std::move(RoleImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Role SyntacticElementFactoryImpl::make_transitive_closure(const Role& role) {
    element::Role_Ptr role_ptr = m_cache.role_element_cache().at(role.compute_repr());
    element::Role_Ptr result_ptr = std::make_shared<element::TransitiveClosureRole>(*m_vocabulary_info, role_ptr);
    result_ptr = m_cache.role_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Role(std::move(RoleImpl(*m_vocabulary_info, std::move(result_ptr))));
}

Role SyntacticElementFactoryImpl::make_transitive_reflexive_closure(const Role& role) {
    element::Role_Ptr role_ptr = m_cache.role_element_cache().at(role.compute_repr());
    element::Role_Ptr result_ptr = std::make_shared<element::TransitiveReflexiveClosureRole>(*m_vocabulary_info, role_ptr);
    result_ptr = m_cache.role_element_cache().emplace(result_ptr->compute_repr(), result_ptr).first->second;
    return Role(std::move(RoleImpl(*m_vocabulary_info, std::move(result_ptr))));
}

const VocabularyInfoImpl* SyntacticElementFactoryImpl::get_vocabulary_info() const {
    return m_vocabulary_info.get();
}

}
}
