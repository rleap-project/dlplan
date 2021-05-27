#ifndef DLP_SRC_CORE_INSTANCE_INFO_IMPL_H_
#define DLP_SRC_CORE_INSTANCE_INFO_IMPL_H_

#include <unordered_map>
#include <string>
#include <vector>

#include "atom.h"
#include "types.h"


namespace dlp {
namespace core {
class StateImpl;

/**
 * InstanceInfo stores Atom related information and provides functionality for state transformation.
 *
 * TODO(dominik): Since primitive roles and concept depend on a certain type of predicates
 * it can make sense to sort the atoms by name for improved cache locality.
 * Can benchmark this first by planning with sorted and unsorted atoms.
 */
class InstanceInfoImpl {
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
     * Indices of static atoms, i.e., atoms that do not change and remain true forever.
     */
    Index_Vec m_static_atom_idxs;
    /**
     * All atoms.
     */
    std::vector<AtomImpl> m_atoms;

public:
    InstanceInfoImpl() = default;
    ~InstanceInfoImpl() = default;

    /**
     * Methods for initializing the TaskInfo successively.
     */
    AtomImpl add_atom(const std::string &predicate_name, const Name_Vec &object_names, bool is_static);
    /**
     * Constructs a state from atom indices by extending with the static and goal atoms of the instance.
     */
    StateImpl convert_state(std::shared_ptr<InstanceInfoImpl> info, const Index_Vec& atom_idxs);

    bool exists_predicate_name(const std::string& name) const;
    unsigned predicate_idx(const std::string& name) const;
    unsigned predicate_arity(unsigned predicate_idx) const;
    /**
     * Getters
     */
    const AtomImpl& atom(unsigned atom_idx) const;
};

}
}

#endif