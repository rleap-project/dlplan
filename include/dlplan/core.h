#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_H_

#include <memory>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "pimpl.h"
#include "types.h"
#include "dynamic_bitset.h"
#include "phmap/phmap.h"


namespace dlplan::core {
class SyntacticElementFactoryImpl;
class InstanceInfoRoot;
class InstanceInfoImpl;
class VocabularyInfoRoot;
class VocabularyInfoImpl;
class SyntacticElementFactory;
class InstanceInfo;
class VocabularyInfo;
namespace element {
    template<typename T>
    class Element;
    class Concept;
    class Role;
    class Numerical;
    class Boolean;
}

class ConceptDenotationFlatSet {
private:
    int m_num_objects;
    phmap::flat_hash_set<int> m_data;

public:
    explicit ConceptDenotationFlatSet(int num_objects);
    ConceptDenotationFlatSet(const ConceptDenotationFlatSet& other);
    ConceptDenotationFlatSet& operator=(const ConceptDenotationFlatSet& other);
    ConceptDenotationFlatSet(ConceptDenotationFlatSet&& other);
    ConceptDenotationFlatSet& operator=(ConceptDenotationFlatSet&& other);
    ~ConceptDenotationFlatSet();

    ConceptDenotationFlatSet& operator&=(const ConceptDenotationFlatSet& other);
    ConceptDenotationFlatSet& operator|=(const ConceptDenotationFlatSet& other);
    ConceptDenotationFlatSet& operator-=(const ConceptDenotationFlatSet& other);
    ConceptDenotationFlatSet& operator~();

    phmap::flat_hash_set<int>::const_iterator begin() const;
    phmap::flat_hash_set<int>::const_iterator end() const;

    bool contains(int value) const;

    void insert(int value);
    void erase(int value);

    int size() const;
    bool empty() const;
    bool intersects(const ConceptDenotationFlatSet& other) const;
    bool is_subset_of(const ConceptDenotationFlatSet& other) const;

    std::vector<int> to_sorted_vector() const;

    int get_num_objects() const;
};


class RoleDenotationFlatSet {
private:
    int m_num_objects;
    phmap::flat_hash_set<std::pair<int, int>> m_data;

public:
    explicit RoleDenotationFlatSet(int num_objects);
    RoleDenotationFlatSet(const RoleDenotationFlatSet& other);
    RoleDenotationFlatSet& operator=(const RoleDenotationFlatSet& other);
    RoleDenotationFlatSet(RoleDenotationFlatSet&& other);
    RoleDenotationFlatSet& operator=(RoleDenotationFlatSet&& other);
    ~RoleDenotationFlatSet();

    RoleDenotationFlatSet& operator&=(const RoleDenotationFlatSet& other);
    RoleDenotationFlatSet& operator|=(const RoleDenotationFlatSet& other);
    RoleDenotationFlatSet& operator-=(const RoleDenotationFlatSet& other);
    RoleDenotationFlatSet& operator~();

    phmap::flat_hash_set<std::pair<int, int>>::const_iterator begin() const;
    phmap::flat_hash_set<std::pair<int, int>>::const_iterator end() const;

    bool contains(const std::pair<int, int>& value) const;
    void insert(const std::pair<int, int>& value);
    void erase(const std::pair<int, int>& value);

    int size() const;
    bool empty() const;
    bool intersects(const RoleDenotationFlatSet& other) const;
    bool is_subset_of(const RoleDenotationFlatSet& other) const;

    std::vector<std::pair<int, int>> to_sorted_vector() const;

    int get_num_objects() const;
};

class ConceptDenotationBitset {
private:
    int m_num_objects;
    dynamic_bitset::DynamicBitset<unsigned> m_data;

public:
    // Special iterator for bitset representing set of integers
    class const_iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type        = dynamic_bitset::DynamicBitset<unsigned>;
            using const_reference   = const value_type&;

            const_iterator(const_reference data, int num_objects, bool end=false);

            bool operator!=(const const_iterator& other) const;
            bool operator==(const const_iterator& other) const;

            const int& operator*() const;
            // Postfix increment
            const_iterator operator++(int);
            // Prefix increment
            const_iterator& operator++();

        private:
            const_reference m_data;
            int m_num_objects;
            int m_index;

        private:
            void seek_next();
    };

    explicit ConceptDenotationBitset(int num_objects);
    ConceptDenotationBitset(const ConceptDenotationBitset& other);
    ConceptDenotationBitset& operator=(const ConceptDenotationBitset& other);
    ConceptDenotationBitset(ConceptDenotationBitset&& other);
    ConceptDenotationBitset& operator=(ConceptDenotationBitset&& other);
    ~ConceptDenotationBitset();

    ConceptDenotationBitset& operator&=(const ConceptDenotationBitset& other);
    ConceptDenotationBitset& operator|=(const ConceptDenotationBitset& other);
    ConceptDenotationBitset& operator-=(const ConceptDenotationBitset& other);
    ConceptDenotationBitset& operator~();

    const_iterator begin() const;
    const_iterator end() const;

    bool contains(int value) const;

    void insert(int value);
    void erase(int value);

    int size() const;
    bool empty() const;
    bool intersects(const ConceptDenotationBitset& other) const;
    bool is_subset_of(const ConceptDenotationBitset& other) const;

    std::vector<int> to_sorted_vector() const;

    int get_num_objects() const;
};
using ConceptDenotation = ConceptDenotationBitset;

class RoleDenotationBitset {
private:
    int m_num_objects;
    dynamic_bitset::DynamicBitset<unsigned> m_data;

public:
    // Special iterator for bitset representing set of pairs of ints.
    class const_iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type        = dynamic_bitset::DynamicBitset<unsigned>;
            using const_reference   = const value_type&;

            const_iterator(const_reference data, int num_objects, bool end=false);

            bool operator!=(const const_iterator& other) const;
            bool operator==(const const_iterator& other) const;

            const std::pair<int, int>& operator*() const;
            // Postfix increment
            const_iterator operator++(int);
            // Prefix increment
            const_iterator& operator++();

        private:
            const_reference m_data;
            int m_num_objects;
            std::pair<int, int> m_indices;

        private:
            void seek_next();
    };

    explicit RoleDenotationBitset(int num_objects);
    RoleDenotationBitset(const RoleDenotationBitset& other);
    RoleDenotationBitset& operator=(const RoleDenotationBitset& other);
    RoleDenotationBitset(RoleDenotationBitset&& other);
    RoleDenotationBitset& operator=(RoleDenotationBitset&& other);
    ~RoleDenotationBitset();

    RoleDenotationBitset& operator&=(const RoleDenotationBitset& other);
    RoleDenotationBitset& operator|=(const RoleDenotationBitset& other);
    RoleDenotationBitset& operator-=(const RoleDenotationBitset& other);
    RoleDenotationBitset& operator~();

    const_iterator begin() const;
    const_iterator end() const;

    bool contains(const std::pair<int, int>& value) const;

    void insert(const std::pair<int, int>& value);
    void erase(const std::pair<int, int>& value);

    int size() const;
    bool empty() const;
    bool intersects(const RoleDenotationBitset& other) const;
    bool is_subset_of(const RoleDenotationBitset& other) const;

    std::vector<std::pair<int, int>> to_sorted_vector() const;

    int get_num_objects() const;
};
using RoleDenotation = RoleDenotationBitset;


class Constant {
private:
    std::shared_ptr<const VocabularyInfoRoot> m_root;
    std::string m_name;
    int m_index;

    Constant(std::shared_ptr<const VocabularyInfoRoot> root, const std::string& name, int index);
    friend class VocabularyInfoImpl;

public:
    Constant() = delete;
    Constant(const Constant& other);
    Constant& operator=(const Constant& other);
    Constant(Constant&& other);
    Constant& operator=(Constant&& other);
    ~Constant();

    bool operator==(const Constant& other) const;
    bool operator!=(const Constant& other) const;

    int get_index() const;
    const std::string& get_name() const;
    std::shared_ptr<const VocabularyInfoRoot> get_vocabulary_info_root() const;
};


/**
 * A Predicate belongs to a specific vocabulary of a planning domain.
 */
class Predicate {
private:
    std::shared_ptr<const VocabularyInfoRoot> m_root;
    std::string m_name;
    int m_index;
    int m_arity;

    Predicate(std::shared_ptr<const VocabularyInfoRoot> root, const std::string& name, int index, int arity);
    friend class VocabularyInfoImpl;

public:
    Predicate(const Predicate& other);
    Predicate& operator=(const Predicate& other);
    Predicate(Predicate&& other);
    Predicate& operator=(Predicate&& other);
    ~Predicate();

    bool operator==(const Predicate& other) const;
    bool operator!=(const Predicate& other) const;

    /**
     * Getters.
     */
    int get_index() const;
    const std::string& get_name() const;
    int get_arity() const;
    std::shared_ptr<const VocabularyInfoRoot> get_vocabulary_info_root() const;
};


/**
 * An Object belongs to a specific instance.
 */
class Object {
private:
    std::shared_ptr<const InstanceInfoRoot> m_root;
    std::string m_name;
    int m_index;

    Object(std::shared_ptr<const InstanceInfoRoot> root, const std::string& name, int index);
    friend class InstanceInfoImpl;

public:
    Object(const Object& other);
    Object& operator=(const Object& other);
    Object(Object&& other);
    Object& operator=(Object&& other);
    ~Object();

    bool operator==(const Object& other) const;
    bool operator!=(const Object& other) const;

    int get_index() const;
    const std::string& get_name() const;
    std::shared_ptr<const InstanceInfoRoot> get_instance_info_root() const;
};


/**
 * An Atom belongs to a specific instance.
 */
class Atom {
private:
    std::shared_ptr<const InstanceInfoRoot> m_root;
    std::string m_name;
    int m_index;
    Predicate m_predicate;
    std::vector<Object> m_objects;
    bool m_is_static;

    Atom(std::shared_ptr<const InstanceInfoRoot> root,
        const std::string& name,
        int index,
        const Predicate& predicate,
        const std::vector<Object> &objects,
        bool is_static);
    friend class InstanceInfoImpl;

public:
    Atom(const Atom& other);
    Atom& operator=(const Atom& other);
    Atom(Atom&& other);
    Atom& operator=(Atom&& other);
    ~Atom();

    bool operator==(const Atom& other) const;
    bool operator!=(const Atom& other) const;

    /**
     * Getters.
     */
    std::string get_name() const;
    int get_index() const;
    const Predicate& get_predicate() const;
    const std::vector<Object>& get_objects() const;
    const Object& get_object(int pos) const;
    bool get_is_static() const;
    std::shared_ptr<const InstanceInfoRoot> get_instance_info_root() const;
};


/**
 * A State contains consists of atoms including all static atoms.
 */
class State {
private:
    std::shared_ptr<const InstanceInfo> m_instance_info;
    Index_Vec m_atom_idxs;

    phmap::flat_hash_map<int, std::vector<int>> m_per_predicate_idx_atom_idxs;

public:
    State(std::shared_ptr<const InstanceInfo> instance_info, const std::vector<Atom>& atoms);
    /**
     * Expert interface to construct states without the overhead
     * of copying Atoms but instead working on indices directly.
     */
    State(std::shared_ptr<const InstanceInfo> instance_info, const Index_Vec& atom_idxs);
    State(const State& other);
    State& operator=(const State& other);
    State(State&& other);
    State& operator=(State&& other);
    ~State();

    bool operator==(const State& other) const;
    bool operator!=(const State& other) const;

    /**
     * Computes string-like representation of the state.
     */
    std::string str() const;

    /**
     * Getters.
     */
    std::shared_ptr<const InstanceInfo> get_instance_info() const;
    const Index_Vec& get_atom_idxs() const;
    const phmap::flat_hash_map<int, std::vector<int>>& get_per_predicate_idx_atom_idxs() const;
};

/**
 * VocabularyInfoRoot is parent of VocabularyInfo
 * and used to define parent-child relationship
 * between VocabularyInfo and other objects.
 */
struct VocabularyInfoRoot { };

/**
 * VocabularyInfo stores information related to the planning domain.
 */
class VocabularyInfo {
private:
    pimpl<VocabularyInfoImpl> m_pImpl;

public:
    VocabularyInfo();
    VocabularyInfo(const VocabularyInfo& other);
    VocabularyInfo& operator=(const VocabularyInfo& other);
    VocabularyInfo(VocabularyInfo&& other);
    VocabularyInfo& operator=(VocabularyInfo&& other);
    ~VocabularyInfo();

    const Predicate& add_predicate(const std::string &name, int arity);

    const Constant& add_constant(const std::string& name);

    bool exists_predicate(const Predicate& predicate) const;
    bool exists_predicate_name(const std::string& name) const;
    const std::vector<Predicate>& get_predicates() const;
    int get_predicate_idx(const std::string& name) const;
    const Predicate& get_predicate(int index) const;
    bool exists_constant(const Constant& constant) const;
    bool exists_constant_name(const std::string& name) const;
    int get_constant_idx(const std::string& name) const;
    const Constant& get_constant(int index) const;
    const std::vector<Constant>& get_constants() const;
    std::shared_ptr<const VocabularyInfoRoot> get_vocabulary_info_root() const;
};

/**
 * InstanceInfoRoot is parent of InstanceInfo
 * and used to define parent-child relationship
 * between InstanceInfo and other objects.
 */
struct InstanceInfoRoot { };

/**
 * InstanceInfo stores information related to the planning instance.
 */
class InstanceInfo {
private:
    pimpl<InstanceInfoImpl> m_pImpl;

public:
    InstanceInfo() = delete;
    InstanceInfo(std::shared_ptr<const VocabularyInfo> vocabulary_info);
    InstanceInfo(const InstanceInfo& other);
    InstanceInfo& operator=(const InstanceInfo& other);
    InstanceInfo(InstanceInfo&& other);
    InstanceInfo& operator=(InstanceInfo&& other);
    ~InstanceInfo();

    /**
     * Alternative 1 to add atoms.
     */
    const Object& add_object(const std::string& object_name);
    const Atom& add_atom(const Predicate& predicate, const std::vector<Object>& objects);
    const Atom& add_static_atom(const Predicate& predicate, const std::vector<Object>& objects);

    /**
     * Alternative 2 to add atoms.
     */
    const Atom& add_atom(const std::string& predicate_name, const Name_Vec& object_names);
    const Atom& add_static_atom(const std::string& predicate_name, const Name_Vec& object_names);

    /**
     * Getters.
     */
    bool exists_atom(const Atom& atom) const;
    const std::vector<Atom>& get_atoms() const;
    const Atom& get_atom(int index) const;
    int get_atom_idx(const std::string& name) const;
    bool exists_object(const Object& object) const;
    bool exists_object(const std::string name) const;
    const std::vector<Object>& get_objects() const;
    const Object& get_object(int index) const;
    int get_object_idx(const std::string& name) const;
    int get_num_objects() const;
    std::shared_ptr<const VocabularyInfo> get_vocabulary_info() const;
    const Index_Vec& get_static_atom_idxs() const;
    const phmap::flat_hash_map<int, std::vector<int>>& get_per_predicate_idx_static_atom_idxs() const;
    const ConceptDenotation& get_top_concept() const;
    const RoleDenotation& get_top_role() const;
    std::shared_ptr<const InstanceInfoRoot> get_instance_info_root() const;
};


/**
 * Abstract base class of any Element.
 */
template<typename T>
class Element {
protected:
    std::shared_ptr<const VocabularyInfo> m_vocabulary_info;

protected:
    Element(std::shared_ptr<const VocabularyInfo> vocabulary_info);

public:
    virtual ~Element();

    /**
     * Evaluates the element for a state given as a vector of atom indices.
     */
    virtual T evaluate(const State& state) const = 0;

    /**
     * Returns the complexity of the element
     * measured in the size of the abstract syntax tree.
     */
    virtual int compute_complexity() const = 0;

    /**
     * Returns a canonical string representation.
     */
    virtual std::string compute_repr() const = 0;

    /**
     * Getters.
     */
    std::shared_ptr<const VocabularyInfo> get_vocabulary_info() const;
};


/**
 * Concept evaluates to ConceptDenotation.
 */
class Concept : public Element<ConceptDenotation> {
private:
    std::shared_ptr<const element::Concept> m_element;

    Concept(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const element::Concept>&& concept);
    friend class SyntacticElementFactoryImpl;

public:
    Concept(const Concept& other);
    Concept& operator=(const Concept& other);
    Concept(Concept&& other);
    Concept& operator=(Concept&& other);
    ~Concept() override;

    ConceptDenotation evaluate(const State& state) const override;

    int compute_complexity() const override;

    std::string compute_repr() const override;

    std::shared_ptr<const element::Concept> get_element() const;
};


/**
 * Concept evaluates to RoleDenotation.
 */
class Role : public Element<RoleDenotation> {
private:
    std::shared_ptr<const element::Role> m_element;

    Role(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const element::Role>&& role);
    friend class SyntacticElementFactoryImpl;

public:
    Role(const Role& other);
    Role& operator=(const Role& other);
    Role(Role&& other);
    Role& operator=(Role&& other);
    ~Role() override;

    RoleDenotation evaluate(const State& state) const override;

    int compute_complexity() const override;

    std::string compute_repr() const override;

    std::shared_ptr<const element::Role> get_element() const;
};


/**
 * Numerical evaluates to int.
 */
class Numerical : public Element<int> {
private:
    std::shared_ptr<const element::Numerical> m_element;

    Numerical(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const element::Numerical>&& numerical);
    friend class SyntacticElementFactoryImpl;

public:
    Numerical(const Numerical& other);
    Numerical& operator=(const Numerical& other);
    Numerical(Numerical&& other);
    Numerical& operator=(Numerical&& other);
    ~Numerical() override;

    int evaluate(const State& state) const override;

    int compute_complexity() const override;

    std::string compute_repr() const override;

    std::shared_ptr<const element::Numerical> get_element() const;
};


/**
 * Boolean evaluates to bool.
 */
class Boolean : public Element<bool> {
private:
    std::shared_ptr<const element::Boolean> m_element;

    Boolean(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const element::Boolean>&& boolean);
    friend class SyntacticElementFactoryImpl;

public:
    Boolean(const Boolean& other);
    Boolean& operator=(const Boolean& other);
    Boolean(Boolean&& other);
    Boolean& operator=(Boolean&& other);
    ~Boolean() override;

    bool evaluate(const State& state) const override;

    int compute_complexity() const override;

    std::string compute_repr() const override;

    std::shared_ptr<const element::Boolean> get_element() const;
};


/**
 * The SyntacticElementFactory for creation of syntactically unique elements.
 */
class SyntacticElementFactory {
private:
    pimpl<SyntacticElementFactoryImpl> m_pImpl;

public:
    SyntacticElementFactory(std::shared_ptr<const VocabularyInfo> vocabulary_info);
    SyntacticElementFactory(const SyntacticElementFactory& other);
    SyntacticElementFactory& operator=(const SyntacticElementFactory& other);
    SyntacticElementFactory(SyntacticElementFactory&& other);
    SyntacticElementFactory& operator=(SyntacticElementFactory&& other);
    ~SyntacticElementFactory();

    const VocabularyInfo* get_vocabulary_info() const;

    /**
     * Returns a Concept if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    Concept parse_concept(const std::string &description);

    /**
     * Returns a Role if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    Role parse_role(const std::string &description);

    /**
     * Returns a Numerical if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    Numerical parse_numerical(const std::string &description);

    /**
     * Returns a Boolean if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    Boolean parse_boolean(const std::string &description);


    Boolean make_empty_boolean(const Concept& concept);
    Boolean make_empty_boolean(const Role& role);
    Boolean make_concept_inclusion_boolean(const Concept& concept_left, const Concept& concept_right);
    Boolean make_role_inclusion_boolean(const Role& role_left, const Role& role_right);
    Boolean make_nullary_boolean(const Predicate& predicate);

    Concept make_all_concept(const Role& role, const Concept& concept);
    Concept make_and_concept(const Concept& concept_left, const Concept& concept_right);
    Concept make_bot_concept();
    Concept make_diff_concept(const Concept& concept_left, const Concept& concept_right);
    Concept make_equal_concept(const Role& role_left, const Role& role_right);
    Concept make_not_concept(const Concept& concept);
    Concept make_one_of_concept(const Constant& constant);
    Concept make_or_concept(const Concept& concept_left, const Concept& concept_right);
    Concept make_projection_concept(const Role& role, int pos);
    Concept make_primitive_concept(const Predicate& predicate, int pos);
    Concept make_some_concept(const Role& role, const Concept& concept);
    Concept make_subset_concept(const Role& role_left, const Role& role_right);
    Concept make_top_concept();

    Numerical make_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to);
    Numerical make_count(const Concept& concept);
    Numerical make_count(const Role& role);
    Numerical make_role_distance(const Role& role_from, const Role& role, const Role& role_to);
    Numerical make_sum_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to);
    Numerical make_sum_role_distance(const Role& role_from, const Role& role, const Role& role_to);

    Role make_and_role(const Role& role_left, const Role& role_right);
    Role make_compose_role(const Role& role_left, const Role& role_right);
    Role make_diff_role(const Role& role_left, const Role& role_right);
    Role make_identity_role(const Concept& concept);
    Role make_inverse_role(const Role& role);
    Role make_not_role(const Role& role);
    Role make_or_role(const Role& role_left, const Role& role_right);
    Role make_primitive_role(const Predicate& predicate, int pos_1, int pos_2);
    Role make_restrict_role(const Role& role, const Concept& concept);
    Role make_top_role();
    Role make_transitive_closure(const Role& role);
    Role make_transitive_reflexive_closure(const Role& role);
};

}

#include "core.tpp"

#endif
