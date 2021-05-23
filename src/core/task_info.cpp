#include "task_info.h"

namespace dlp {

void TaskInfo::add_atom(const std::string &predicate_name, const Name_Vec &object_names) {

}

void TaskInfo::set_constant_atoms(const Index_Vec& constant_atom_idxs) {
    m_constant_atom_idxs = constant_atom_idxs;
}

void TaskInfo::set_goal_atoms(const Index_Vec& goal_atom_idxs) {
    m_goal_atom_idxs = goal_atom_idxs;
}

bool TaskInfo::exists_predicate_name(const std::string& name) const {
    return m_predicate_name_to_predicate_idx.find(name) != m_predicate_name_to_predicate_idx.end();
}

bool TaskInfo::predicate_idx(const std::string& name) const {
    return m_predicate_name_to_predicate_idx.at(name);
}

unsigned TaskInfo::predicate_arity(unsigned predicate_idx) const {
    return m_predicate_arities[predicate_idx];
}

const Index_Vec& TaskInfo::goal_atom_idxs() const {
    return m_goal_atom_idxs;
}

const Index_Vec& TaskInfo::constant_atom_idxs() const {
    return m_constant_atom_idxs;
}

const std::vector<Atom>& TaskInfo::atoms() const {
    return m_atoms;
}

}