#include "element_factory_impl.h"
#include "lisp/parser.h"


namespace dlp {

ElementFactoryImpl::ElementFactoryImpl() {
}

void ElementFactoryImpl::add_atom(const std::string &predicate_name,
    Name_Vec &object_names,
    bool constant) {
}

void ElementFactoryImpl::add_atom(const std::string &predicate_name,
    unsigned predicate_idx,
    Name_Vec &object_names,
    Index_Vec &object_idxs,
    bool constant) {
}

ConceptElement_Ptr ElementFactoryImpl::make_concept_element(const std::string &description) {
    lisp::Expression_Ptr expression = lisp::Parser().parse(description);
    return expression->make_concept_element(m_task_info, m_cache);
}

RoleElement_Ptr ElementFactoryImpl::make_role_element(const std::string &description) {
    lisp::Expression_Ptr expression = lisp::Parser().parse(description);
    return expression->make_role_element(m_task_info, m_cache);
}

NumericalElement_Ptr ElementFactoryImpl::make_numerical_element(const std::string &description) {
    lisp::Expression_Ptr expression = lisp::Parser().parse(description);
    return expression->make_numerical_element(m_task_info, m_cache);
}

BooleanElement_Ptr ElementFactoryImpl::make_boolean_element(const std::string &description) {
    lisp::Expression_Ptr expression = lisp::Parser().parse(description);
    return expression->make_boolean_element(m_task_info, m_cache);
}

}
