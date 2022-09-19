#ifndef DLPLAN_SRC_CORE_INSTANCE_INFO_IMPL_H_
#define DLPLAN_SRC_CORE_INSTANCE_INFO_IMPL_H_

#include <unordered_map>
#include <string>
#include <vector>

#include "types.h"

#include "../../include/dlplan/core.h"


namespace dlplan::core {
class StateImpl;
class VocabularyInfoImpl;

class InstanceInfoImpl {
private:
    std::shared_ptr<const VocabularyInfo> m_vocabulary_info;
    int m_index;

    /**
     * All atoms.
     * TODO(dominik): sorted atoms by predicate can lead to more efficient evaluation of primitive role and concept.
     */
    std::unordered_map<std::string, unsigned> m_atom_name_to_atom_idx;
    std::vector<Atom> m_atoms;

    std::unordered_map<std::string, unsigned> m_static_atom_name_to_static_atom_idx;
    std::vector<Atom> m_static_atoms;
    phmap::flat_hash_map<int, std::vector<int>> m_per_predicate_idx_static_atom_idxs;

    /**
     * All objects.
     */
    std::unordered_map<std::string, unsigned> m_object_name_to_object_idx;
    std::vector<Object> m_objects;
    /**
     * Precomputed set and vector representations for evaluation results of primitives.
     */
    mutable ConceptDenotation m_top_concept;
    mutable RoleDenotation m_top_role;

    const Atom& add_atom(const std::string &predicate_name, const Name_Vec &object_names, bool is_static);
    const Atom& add_atom(const Predicate& predicate, const std::vector<Object>& objects, bool is_static);

public:
    explicit InstanceInfoImpl(std::shared_ptr<const VocabularyInfo> vocabulary_info, int index=-1);
    ~InstanceInfoImpl() = default;

    const Object& add_object(const std::string& object_name);

    const Atom& add_atom(const Predicate& predicate, const std::vector<Object>& objects);
    const Atom& add_static_atom(const Predicate& predicate, const std::vector<Object>& objects);

    const Atom& add_atom(const std::string &predicate_name, const Name_Vec &object_names);
    const Atom& add_static_atom(const std::string& predicate_name, const Name_Vec& object_names);

    void set_index(int index);

    int get_index() const;
    bool exists_atom(const Atom& atom) const;
    const std::vector<Atom>& get_atoms() const;
    const std::vector<Atom>& get_static_atoms() const;
    const Atom& get_atom(int atom_idx) const;
    int get_atom_idx(const std::string& name) const;
    bool exists_object(const Object& object) const;
    bool exists_object(const std::string name) const;
    const std::vector<Object>& get_objects() const;
    const Object& get_object(int object_idx) const;
    int get_object_idx(const std::string& object_name) const;
    int get_num_objects() const;
    std::shared_ptr<const VocabularyInfo> get_vocabulary_info() const;
    const phmap::flat_hash_map<int, std::vector<int>>& get_per_predicate_idx_static_atom_idxs() const;
    const ConceptDenotation& get_top_concept() const;
    const RoleDenotation& get_top_role() const;
};

}

#endif