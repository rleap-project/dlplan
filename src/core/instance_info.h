#ifndef DLP_SRC_CORE_INSTANCE_INFO_IMPL_H_
#define DLP_SRC_CORE_INSTANCE_INFO_IMPL_H_

#include <unordered_map>
#include <string>
#include <vector>

#include "predicate.h"
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
    std::unordered_map<std::string, unsigned> m_atom_name_to_atom_idx;
    /**
     * Indices of static atoms, i.e., atoms that do not change and remain true forever.
     */
    Index_Vec m_static_atom_idxs;
    /**
     * All atoms.
     */
    std::vector<AtomImpl> m_atoms;
    /**
     * All predicates.
     */
    std::vector<PredicateImpl> m_predicates;

public:
    InstanceInfoImpl() = default;
    ~InstanceInfoImpl() = default;

    /**
     * Adds an atom that may have varying evaluation depending on the state.
     */
    AtomImpl add_atom(const std::string &predicate_name, const Name_Vec &object_names);

    /**
     * Adds an atom that remains true forever.
     */
    AtomImpl add_static_atom(const std::string& predicate_name, const Name_Vec& object_names);

    /**
     * Construct a state from textual information by first applying the index mapping and the calling convert_state.
     */
    StateImpl parse_state(std::shared_ptr<InstanceInfoImpl> info, const Name_Vec& atom_names) const;
    /**
     * Constructs a state from atom indices by extending with the static and goal atoms of the instance.
     */
    StateImpl convert_state(std::shared_ptr<InstanceInfoImpl> info, const std::vector<AtomImpl>& atomss) const;
    /**
     * Constructs a state from atom indices by extending with the static and goal atoms of the instance.
     */
    StateImpl convert_state(std::shared_ptr<InstanceInfoImpl> info, const Index_Vec& atom_idxs) const;

    bool exists_predicate_name(const std::string& name) const;
    unsigned get_predicate_idx(const std::string& name) const;
    /**
     * Getters
     */
    const AtomImpl& get_atom(unsigned atom_idx) const;
    const std::vector<AtomImpl>& get_atoms() const;
    const PredicateImpl& get_predicate(unsigned predicate_idx) const;
    const std::vector<PredicateImpl>& get_predicates() const;
};

}
}

#endif