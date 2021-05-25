#include "task_info.h"

namespace dlp {
namespace core {

int TaskInfo::add_atom(const std::string &predicate_name, const Name_Vec &object_names, bool goal) {

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
}
