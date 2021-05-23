#include "element_factory_impl.h"
#include "lisp/parser.h"


namespace dlp {

ElementFactoryImpl::ElementFactoryImpl()
    : m_task_info(std::make_shared<TaskInfo>()) {
}

void ElementFactoryImpl::add_atom(const std::string &predicate_name, const Name_Vec &object_names) {
    m_task_info->add_atom(predicate_name, object_names);
}

void ElementFactoryImpl::set_constant_atoms(const Index_Vec& constant_atom_idxs) {
    m_task_info->set_constant_atoms(constant_atom_idxs);
}

void ElementFactoryImpl::set_goal_atoms(const Index_Vec& goal_atom_idxs) {
    m_task_info->set_goal_atoms(goal_atom_idxs);
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
