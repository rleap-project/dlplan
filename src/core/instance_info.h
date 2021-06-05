#ifndef DLP_SRC_CORE_INSTANCE_INFO_IMPL_H_
#define DLP_SRC_CORE_INSTANCE_INFO_IMPL_H_

#include <unordered_map>
#include <string>
#include <vector>

#include "vocabulary_info.h"
#include "predicate.h"
#include "object.h"
#include "types.h"

#include "../../include/dlp/core.h"


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
class InstanceInfoImpl : public std::enable_shared_from_this<InstanceInfoImpl> {
private:
    const std::shared_ptr<const VocabularyInfoImpl> m_vocabulary_info;

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
    std::vector<Atom> m_atoms;
    /**
     * All objects.
     */
    std::vector<Object> m_objects;

    const Atom& add_atom(const std::string &predicate_name, const Name_Vec &object_names, bool is_static);

public:
    InstanceInfoImpl(const VocabularyInfoImpl& vocabulary_info);
    ~InstanceInfoImpl() = default;

    /**
     * Adds an atom that may have varying evaluation depending on the state.
     */
    const Atom& add_atom(const std::string &predicate_name, const Name_Vec &object_names);

    /**
     * Adds an atom that remains true forever.
     */
    const Atom& add_static_atom(const std::string& predicate_name, const Name_Vec& object_names);

    /**
     * Construct a state from textual information by first applying the index mapping and the calling convert_state.
     */
    State parse_state(const Name_Vec& atom_names) const;
    /**
     * Constructs a state from atom indices by extending with the static and goal atoms of the instance.
     */
    State convert_state(const std::vector<Atom>& atoms) const;
    /**
     * Constructs a state from atom indices by extending with the static and goal atoms of the instance.
     */
    State convert_state(const Index_Vec& atom_idxs) const;

    /**
     * Getters
     */
    const std::vector<Atom>& get_atoms() const;
    const Atom& get_atom(unsigned atom_idx) const;
    const std::vector<Object>& get_objects() const;
    const Object& get_object(unsigned object_idx) const;
    unsigned get_object_idx(const std::string& object_name) const;
    unsigned get_num_objects() const;
    const std::shared_ptr<const VocabularyInfoImpl>& get_vocabulary_info() const;
};

}
}

#endif