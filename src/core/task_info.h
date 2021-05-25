#ifndef DLP_SRC_CORE_TASK_INFO_H_
#define DLP_SRC_CORE_TASK_INFO_H_

#include <unordered_map>
#include <string>
#include <vector>

#include "atom.h"
#include "types.h"


namespace dlp {
namespace core {

class TaskInfo {
private:
    /**
     * Mappings between names and indices of predicates and objects.
     */
    std::unordered_map<std::string, unsigned> m_predicate_name_to_predicate_idx;
    std::unordered_map<std::string, unsigned> m_object_name_to_object_idx;
    std::vector<std::string> m_predicate_idx_to_predicate_name;
    std::vector<std::string> m_object_idx_to_to_object_name;
    /**
     * Predicate arities
     */
    std::vector<unsigned> m_predicate_arities;
    /**
     * Atoms satisfied in the goal.
     */
    Index_Vec m_goal_atom_idxs;
    /**
     * Trivially satisfied atoms.
     */
    Index_Vec m_constant_atom_idxs;
    /**
     * All atoms
     */
    std::vector<Atom> m_atoms;

public:
    /**
     * Methods for initializing the TaskInfo successively.
     */
    int add_atom(const std::string &predicate_name, const Name_Vec &object_names, bool goal);

    bool exists_predicate_name(const std::string& name) const;
    bool predicate_idx(const std::string& name) const;
    unsigned predicate_arity(unsigned predicate_idx) const;

    /**
     * Getters.
     */
    const Index_Vec& goal_atom_idxs() const;
    const Index_Vec& constant_atom_idxs() const;
    const std::vector<Atom>& atoms() const;
};

}
}

#endif