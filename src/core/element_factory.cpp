#include "element_factory.h"
#include "parser/parser.h"
#include "parser/expressions/expression.h"


namespace dlp {
namespace core {

ElementFactoryImpl::ElementFactoryImpl() {
}

element::ConceptElement_Ptr ElementFactoryImpl::make_concept_element(const InstanceInfoImpl& info, const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->make_concept_element(info, m_cache);
}

element::RoleElement_Ptr ElementFactoryImpl::make_role_element(const InstanceInfoImpl& info, const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->make_role_element(info, m_cache);
}

element::NumericalElement_Ptr ElementFactoryImpl::make_numerical_element(const InstanceInfoImpl& info, const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->make_numerical_element(info, m_cache);
}

element::BooleanElement_Ptr ElementFactoryImpl::make_boolean_element(const InstanceInfoImpl& info, const std::string &description) {
    parser::Expression_Ptr expression = parser::Parser().parse(description);
    return expression->make_boolean_element(info, m_cache);
}

}
}
