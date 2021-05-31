#include "element_factory.h"
#include "parser/parser.h"
#include "parser/expressions/expression.h"
#include "elements/numericals/count.h"
#include "elements/concepts/primitive.h"
#include "elements/concepts/and.h"
#include "elements/roles/primitive.h"


namespace dlp {
namespace core {

SyntacticElementFactoryImpl::SyntacticElementFactoryImpl() {
}

element::ConceptElement_Ptr SyntacticElementFactoryImpl::parse_concept_element(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->parse_concept_element(m_cache);
}

element::RoleElement_Ptr SyntacticElementFactoryImpl::parse_role_element(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->parse_role_element(m_cache);
}

element::NumericalElement_Ptr SyntacticElementFactoryImpl::parse_numerical_element(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->parse_numerical_element(m_cache);
}

element::BooleanElement_Ptr SyntacticElementFactoryImpl::parse_boolean_element(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->parse_boolean_element(m_cache);
}

element::BooleanElement_Ptr SyntacticElementFactoryImpl::make_empty_boolean_element(element::ConceptElement_Ptr concept) {
}

element::BooleanElement_Ptr SyntacticElementFactoryImpl::make_empty_boolean_element(element::RoleElement_Ptr role) {

}

element::ConceptElement_Ptr SyntacticElementFactoryImpl::make_all_concept_element(element::RoleElement_Ptr role, element::ConceptElement_Ptr concept) {

}

element::ConceptElement_Ptr SyntacticElementFactoryImpl::make_and_concept_element(element::ConceptElement_Ptr element1, element::ConceptElement_Ptr element2) {
    element::ConceptElement_Ptr value = std::make_shared<element::AndConceptElement>(element1, element2);
    return m_cache.concept_element_cache().insert(std::make_pair(value->compute_repr(), std::move(value))).first->second;
}

element::ConceptElement_Ptr SyntacticElementFactoryImpl::make_bot_concept_element() {

}

element::ConceptElement_Ptr SyntacticElementFactoryImpl::make_diff_concept_element(element::ConceptElement_Ptr concept_left, element::ConceptElement_Ptr concept_right) {

}

element::ConceptElement_Ptr SyntacticElementFactoryImpl::make_not_concept_element(element::ConceptElement_Ptr concept) {

}

element::ConceptElement_Ptr SyntacticElementFactoryImpl::make_one_of_concept_element(unsigned pos) {

}

element::ConceptElement_Ptr SyntacticElementFactoryImpl::make_or_concept_element(element::ConceptElement_Ptr concept_left, element::ConceptElement_Ptr concept_right) {

}

element::ConceptElement_Ptr SyntacticElementFactoryImpl::make_primitive_concept_element(const std::string& name, unsigned pos) {
    element::ConceptElement_Ptr value = std::make_shared<element::PrimitiveConceptElement>(name, pos);
    return m_cache.concept_element_cache().insert(std::make_pair(value->compute_repr(), std::move(value))).first->second;
}

element::ConceptElement_Ptr SyntacticElementFactoryImpl::make_some_concept_element(element::RoleElement_Ptr role, element::ConceptElement_Ptr concept) {

}

element::ConceptElement_Ptr SyntacticElementFactoryImpl::make_subset_concept_element(element::RoleElement_Ptr role_left, element::RoleElement_Ptr role_right) {

}

element::ConceptElement_Ptr SyntacticElementFactoryImpl::make_top_concept_element() {

}

element::NumericalElement_Ptr SyntacticElementFactoryImpl::make_concept_distance_element(element::ConceptElement_Ptr concept_from, element::RoleElement_Ptr role, element::ConceptElement_Ptr concept_to) {

}

element::NumericalElement_Ptr SyntacticElementFactoryImpl::make_count_element(element::ConceptElement_Ptr element) {
    element::NumericalElement_Ptr value = std::make_shared<element::CountNumericalElement<element::ConceptElement_Ptr>>(element);
    return m_cache.numerical_element_cache().insert(std::make_pair(value->compute_repr(), std::move(value))).first->second;
}

element::NumericalElement_Ptr SyntacticElementFactoryImpl::make_count_element(element::RoleElement_Ptr element) {
    element::NumericalElement_Ptr value = std::make_shared<element::CountNumericalElement<element::RoleElement_Ptr>>(element);
    return m_cache.numerical_element_cache().insert(std::make_pair(value->compute_repr(), std::move(value))).first->second;
}

element::NumericalElement_Ptr SyntacticElementFactoryImpl::make_role_distance_element(element::RoleElement_Ptr role_from, element::RoleElement_Ptr role, element::RoleElement_Ptr role_to) {

}

element::NumericalElement_Ptr SyntacticElementFactoryImpl::make_sum_concept_distance_element(element::ConceptElement_Ptr concept_from, element::RoleElement_Ptr role, element::ConceptElement_Ptr concept_to) {

}

element::NumericalElement_Ptr SyntacticElementFactoryImpl::make_sum_role_distance_element(element::RoleElement_Ptr role_from, element::RoleElement_Ptr role, element::RoleElement_Ptr role_to) {

}

element::RoleElement_Ptr SyntacticElementFactoryImpl::make_and_role_element(element::RoleElement_Ptr role_left, element::RoleElement_Ptr role_right) {

}

element::RoleElement_Ptr SyntacticElementFactoryImpl::make_compose_role_element(element::RoleElement_Ptr role_left, element::RoleElement_Ptr role_right) {

}

element::RoleElement_Ptr SyntacticElementFactoryImpl::make_diff_role_element(element::RoleElement_Ptr role_left, element::RoleElement_Ptr role_right) {

}

element::RoleElement_Ptr SyntacticElementFactoryImpl::make_identity_role_element(element::ConceptElement_Ptr concept) {

}

element::RoleElement_Ptr SyntacticElementFactoryImpl::make_inverse_role_element(element::RoleElement_Ptr role) {

}

element::RoleElement_Ptr SyntacticElementFactoryImpl::make_not_role_element(element::RoleElement_Ptr role) {

}

element::RoleElement_Ptr SyntacticElementFactoryImpl::make_or_role_element(element::RoleElement_Ptr role_left, element::RoleElement_Ptr role_right) {

}

element::RoleElement_Ptr SyntacticElementFactoryImpl::make_primitive_role_element(const std::string& name, unsigned pos_1, unsigned pos_2) {
    element::RoleElement_Ptr value = std::make_shared<element::PrimitiveRoleElement>(name, pos_1, pos_2);
    return m_cache.role_element_cache().insert(std::make_pair(value->compute_repr(), std::move(value))).first->second;
}

element::RoleElement_Ptr SyntacticElementFactoryImpl::make_restrict_role_element(element::RoleElement_Ptr role, element::ConceptElement_Ptr concept) {

}

element::RoleElement_Ptr SyntacticElementFactoryImpl::make_top_role_element() {

}

element::RoleElement_Ptr SyntacticElementFactoryImpl::make_transitive_closure_element(element::RoleElement_Ptr role) {

}

element::RoleElement_Ptr SyntacticElementFactoryImpl::make_transitive_reflexive_closure_element(element::RoleElement_Ptr role) {

}

}
}
