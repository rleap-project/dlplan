#include "element_factory.h"
#include "parser/parser.h"
#include "parser/expressions/expression.h"
#include "elements/numericals/count.h"
#include "elements/concepts/primitive.h"
#include "elements/concepts/and.h"
#include "elements/roles/primitive.h"


namespace dlp {
namespace core {

ElementFactoryImpl::ElementFactoryImpl() {
}

element::ConceptElement_Ptr ElementFactoryImpl::parse_concept_element(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->parse_concept_element(m_cache);
}

element::RoleElement_Ptr ElementFactoryImpl::parse_role_element(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->parse_role_element(m_cache);
}

element::NumericalElement_Ptr ElementFactoryImpl::parse_numerical_element(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->parse_numerical_element(m_cache);
}

element::BooleanElement_Ptr ElementFactoryImpl::parse_boolean_element(const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->parse_boolean_element(m_cache);
}

element::NumericalElement_Ptr ElementFactoryImpl::make_count_element(element::ConceptElement_Ptr element) {
    element::NumericalElement_Ptr value = std::make_shared<element::CountNumericalElement<element::ConceptElement_Ptr>>(element);
    return m_cache.numerical_element_cache().insert(std::make_pair(value->repr(), std::move(value))).first->second;
}

element::NumericalElement_Ptr ElementFactoryImpl::make_count_element(element::RoleElement_Ptr element) {
    element::NumericalElement_Ptr value = std::make_shared<element::CountNumericalElement<element::RoleElement_Ptr>>(element);
    return m_cache.numerical_element_cache().insert(std::make_pair(value->repr(), std::move(value))).first->second;
}

element::ConceptElement_Ptr ElementFactoryImpl::make_primitive_concept_element(const std::string& name, unsigned pos) {
    element::ConceptElement_Ptr value = std::make_shared<element::PrimitiveConceptElement>(name, pos);
    return m_cache.concept_element_cache().insert(std::make_pair(value->repr(), std::move(value))).first->second;
}

element::ConceptElement_Ptr ElementFactoryImpl::make_and_concept_element(element::ConceptElement_Ptr element1, element::ConceptElement_Ptr element2) {
    element::ConceptElement_Ptr value = std::make_shared<element::AndConceptElement>(element1, element2);
    return m_cache.concept_element_cache().insert(std::make_pair(value->repr(), std::move(value))).first->second;
}

element::RoleElement_Ptr ElementFactoryImpl::make_primitive_role_element(const std::string& name, unsigned pos_1, unsigned pos_2) {
    element::RoleElement_Ptr value = std::make_shared<element::PrimitiveRoleElement>(name, pos_1, pos_2);
    return m_cache.role_element_cache().insert(std::make_pair(value->repr(), std::move(value))).first->second;
}

}
}
