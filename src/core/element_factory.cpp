#include "element_factory.h"
#include "parser/parser.h"
#include "parser/expressions/expression.h"
#include "elements/numericals/count.h"
#include "elements/concepts/primitive.h"
#include "elements/concepts/and.h"


namespace dlp {
namespace core {

ElementFactoryImpl::ElementFactoryImpl() {
}

element::ConceptElement_Ptr ElementFactoryImpl::parse_concept_element(const InstanceInfoImpl& info, const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->parse_concept_element(info, m_cache);
}

element::RoleElement_Ptr ElementFactoryImpl::parse_role_element(const InstanceInfoImpl& info, const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->parse_role_element(info, m_cache);
}

element::NumericalElement_Ptr ElementFactoryImpl::parse_numerical_element(const InstanceInfoImpl& info, const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->parse_numerical_element(info, m_cache);
}

element::BooleanElement_Ptr ElementFactoryImpl::parse_boolean_element(const InstanceInfoImpl& info, const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->parse_boolean_element(info, m_cache);
}

element::NumericalElement_Ptr ElementFactoryImpl::make_count_element(const InstanceInfoImpl& info, element::ConceptElement_Ptr element) {
    element::NumericalElement_Ptr value = std::make_shared<element::CountNumericalElement<element::ConceptElement_Ptr>>(info, element);
    return m_cache.numerical_element_cache().insert_cache_and_retrieve(value->repr(), std::move(value));
}

element::NumericalElement_Ptr ElementFactoryImpl::make_count_element(const InstanceInfoImpl& info, element::RoleElement_Ptr element) {
    element::NumericalElement_Ptr value = std::make_shared<element::CountNumericalElement<element::RoleElement_Ptr>>(info, element);
    return m_cache.numerical_element_cache().insert_cache_and_retrieve(value->repr(), std::move(value));
}

element::ConceptElement_Ptr ElementFactoryImpl::make_primitive_concept_element(const InstanceInfoImpl& info, const std::string& name, unsigned pos) {
    element::ConceptElement_Ptr value = std::make_shared<element::PrimitiveConceptElement>(info, name, pos);
    return m_cache.concept_element_cache().insert_cache_and_retrieve(value->repr(), std::move(value));
}

element::ConceptElement_Ptr ElementFactoryImpl::make_and_concept_element(const InstanceInfoImpl& info, element::ConceptElement_Ptr element1, element::ConceptElement_Ptr element2) {
    element::ConceptElement_Ptr value = std::make_shared<element::AndConceptElement>(info, element1, element2);
    return m_cache.concept_element_cache().insert_cache_and_retrieve(value->repr(), std::move(value));
}

}
}
