#include "task_info.h"

namespace dlp {

bool TaskInfo::exists_predicate_name(const std::string& name) const {
    return m_predicate_name_to_predicate_idx.find(name) != m_predicate_name_to_predicate_idx.end();
}

bool TaskInfo::predicate_idx(const std::string& name) const {
    return m_predicate_name_to_predicate_idx.at(name);
}

unsigned TaskInfo::predicate_arity(unsigned predicate_idx) const {
    return m_predicate_arities[predicate_idx];
}

const Index_Vec& TaskInfo::goal_atoms() const {
    return m_goal_atoms;
}

const Index_Vec& TaskInfo::init_atoms() const {
    return m_init_atoms;
}

}