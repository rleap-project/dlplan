#ifndef DLP_SRC_FEATURE_FACTORY_TASK_INFO_H_
#define DLP_SRC_FEATURE_FACTORY_TASK_INFO_H_

#include <unordered_map>
#include <string>
#include <vector>

#include "atom.h"
#include "types.h"


namespace dlp {

class TaskInfo {
private:
    /**
     * The registered atoms.
     */
    std::vector<Atom> m_atoms;
    /**
     * Mappings between names and indices of predicates and objects.
     */
    std::unordered_map<std::string, unsigned> m_predicate_name_to_predicate_idx;
    std::unordered_map<std::string, unsigned> m_object_name_to_object_idx;
    std::vector<std::string> m_predicate_idx_to_predicate_name;
    std::vector<std::string> m_object_idx_to_to_object_name;
    /**
     * Arities
     */
    std::vector<unsigned> m_predicate_arities;
    /**
     * Goal state information.
     */
    Index_Vec m_goal_atoms;
    /**
     * Initial state information.
     */
    Index_Vec m_init_atoms;
public:

    bool exists_predicate_name(const std::string& name) const;
    bool predicate_idx(const std::string& name) const;
    unsigned predicate_arity(unsigned predicate_idx) const;

    const Index_Vec& goal_atoms() const;
    const Index_Vec& init_atoms() const;
};

}

#endif