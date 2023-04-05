#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_H_

#include "phmap/phmap.h"
#include "utils/pimpl.h"
#include "utils/dynamic_bitset.h"
#include "utils/cache.h"

#include <memory>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>


/**
 * Forward declarations and usings
 */
namespace dlplan::core {
    class SyntacticElementFactoryImpl;
    class SyntacticElementFactory;
    class InstanceInfo;
    class VocabularyInfo;
    class State;
    class ConceptDenotation;
    class RoleDenotation;
    namespace element {
        template<typename T>
        class Element;
        class Concept;
        class Role;
        class Numerical;
        class Boolean;
    }

    using Name_Vec = std::vector<std::string>;
    using Index_Vec = std::vector<int>;
    using Index_Set = std::unordered_set<int>;
    using IndexPair_Vec = std::vector<std::pair<int, int>>;

    using States = std::vector<State>;
    using StatesSet = std::unordered_set<State>;
    using StatePair = std::pair<State, State>;
    using StatePairs = std::vector<StatePair>;

    using ConceptDenotations = std::vector<ConceptDenotation*>;
    using RoleDenotations = std::vector<RoleDenotation*>;
    using BooleanDenotations = std::vector<bool>;
    using NumericalDenotations = std::vector<int>;
}


/**
 * Template specializations
 */
namespace std {
    template<> struct hash<dlplan::core::State> {
        size_t operator()(const dlplan::core::State& state) const noexcept;
    };
    template<>
    struct hash<unique_ptr<dlplan::core::ConceptDenotation>> {
        size_t operator()(const unique_ptr<dlplan::core::ConceptDenotation>& denotation) const noexcept;
    };
    template<>
    struct hash<unique_ptr<dlplan::core::RoleDenotation>> {
        size_t operator()(const unique_ptr<dlplan::core::RoleDenotation>& denotation) const noexcept;
    };
    template<>
    struct hash<unique_ptr<dlplan::core::ConceptDenotations>> {
        size_t operator()(const unique_ptr<dlplan::core::ConceptDenotations>& denotations) const noexcept;
    };
    template<>
    struct hash<unique_ptr<dlplan::core::RoleDenotations>> {
        size_t operator()(const unique_ptr<dlplan::core::RoleDenotations>& denotations) const noexcept;
    };
    template<>
    struct hash<unique_ptr<dlplan::core::BooleanDenotations>> {
        size_t operator()(const unique_ptr<dlplan::core::BooleanDenotations>& denotations) const noexcept;
    };
    template<>
    struct hash<unique_ptr<dlplan::core::NumericalDenotations>> {
        size_t operator()(const unique_ptr<dlplan::core::NumericalDenotations>& denotations) const noexcept;
    };
    template<> struct hash<vector<unsigned>> {
        size_t operator()(const vector<unsigned>& data) const noexcept;
    };
    template<> struct hash<vector<int>> {
        size_t operator()(const vector<int>& data) const noexcept;
    };
    template<> struct hash<std::array<int, 2>> {
        size_t operator()(const std::array<int, 2>& data) const noexcept;
    };
    template<> struct hash<std::array<int, 3>> {
        size_t operator()(const std::array<int, 3>& data) const noexcept;
    };
}


namespace dlplan::core {
class ConceptDenotation {
private:
    int m_num_objects;
    utils::DynamicBitset<unsigned> m_data;

public:
    // Special iterator for bitset representing set of integers
    class const_iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type        = utils::DynamicBitset<unsigned>;
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

    explicit ConceptDenotation(int num_objects);
    ConceptDenotation(const ConceptDenotation& other);
    ConceptDenotation& operator=(const ConceptDenotation& other);
    ConceptDenotation(ConceptDenotation&& other);
    ConceptDenotation& operator=(ConceptDenotation&& other);
    ~ConceptDenotation();

    bool operator==(const ConceptDenotation& other) const ;
    bool operator!=(const ConceptDenotation& other) const;

    ConceptDenotation& operator&=(const ConceptDenotation& other);
    ConceptDenotation& operator|=(const ConceptDenotation& other);
    ConceptDenotation& operator-=(const ConceptDenotation& other);
    ConceptDenotation& operator~();

    const_iterator begin() const;
    const_iterator end() const;

    bool contains(int value) const;
    void set();
    void insert(int value);
    void erase(int value);

    int size() const;
    bool empty() const;
    bool intersects(const ConceptDenotation& other) const;
    bool is_subset_of(const ConceptDenotation& other) const;

    std::vector<int> to_sorted_vector() const;

    std::size_t hash() const;

    std::string str() const;

    int get_num_objects() const;
};


class RoleDenotation {
private:
    int m_num_objects;
    utils::DynamicBitset<unsigned> m_data;

public:
    // Special iterator for bitset representing set of pairs of ints.
    class const_iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type        = utils::DynamicBitset<unsigned>;
            using const_reference   = const value_type&;

            const_iterator(const_reference data, int num_objects, bool end=false);

            bool operator!=(const const_iterator& other) const;
            bool operator==(const const_iterator& other) const;

            const std::pair<int, int>& operator*() const;
            std::pair<int, int>* operator->();
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

    explicit RoleDenotation(int num_objects);
    RoleDenotation(const RoleDenotation& other);
    RoleDenotation& operator=(const RoleDenotation& other);
    RoleDenotation(RoleDenotation&& other);
    RoleDenotation& operator=(RoleDenotation&& other);
    ~RoleDenotation();

    bool operator==(const RoleDenotation& other) const ;
    bool operator!=(const RoleDenotation& other) const;

    RoleDenotation& operator&=(const RoleDenotation& other);
    RoleDenotation& operator|=(const RoleDenotation& other);
    RoleDenotation& operator-=(const RoleDenotation& other);
    RoleDenotation& operator~();

    const_iterator begin() const;
    const_iterator end() const;

    bool contains(const std::pair<int, int>& value) const;
    void set();
    void insert(const std::pair<int, int>& value);
    void erase(const std::pair<int, int>& value);

    int size() const;
    bool empty() const;
    bool intersects(const RoleDenotation& other) const;
    bool is_subset_of(const RoleDenotation& other) const;

    std::vector<std::pair<int, int>> to_sorted_vector() const;

    std::size_t hash() const;

    std::string str() const;

    int get_num_objects() const;
};


/**
 * Compares two std::unique_ptr<T>
 * by comparing objects T.
 */
template<typename T>
struct DerefEqual {
    bool operator()(const T& left, const T& right) const {
        return *left == *right;
    }
};

struct DenotationsCaches {
    // Cache for single denotations.
    std::unordered_set<std::unique_ptr<ConceptDenotation>, std::hash<std::unique_ptr<ConceptDenotation>>, DerefEqual<std::unique_ptr<ConceptDenotation>>> m_c_denot_cache;
    std::unordered_set<std::unique_ptr<RoleDenotation>, std::hash<std::unique_ptr<RoleDenotation>>, DerefEqual<std::unique_ptr<RoleDenotation>>> m_r_denot_cache;
    // Cache for collections of denotations.
    std::unordered_set<std::unique_ptr<BooleanDenotations>, std::hash<std::unique_ptr<BooleanDenotations>>, DerefEqual<std::unique_ptr<BooleanDenotations>>> m_b_denots_cache;
    std::unordered_set<std::unique_ptr<NumericalDenotations>, std::hash<std::unique_ptr<NumericalDenotations>>, DerefEqual<std::unique_ptr<NumericalDenotations>>> m_n_denots_cache;
    std::unordered_set<std::unique_ptr<ConceptDenotations>, std::hash<std::unique_ptr<ConceptDenotations>>, DerefEqual<std::unique_ptr<ConceptDenotations>>> m_c_denots_cache;
    std::unordered_set<std::unique_ptr<RoleDenotations>, std::hash<std::unique_ptr<RoleDenotations>>, DerefEqual<std::unique_ptr<RoleDenotations>>> m_r_denots_cache;
    // Mapping from element index to denotations.
    std::unordered_map<int, BooleanDenotations*> m_b_denots_mapping;
    std::unordered_map<int, NumericalDenotations*> m_n_denots_mapping;
    std::unordered_map<int, ConceptDenotations*> m_c_denots_mapping;
    std::unordered_map<int, RoleDenotations*> m_r_denots_mapping;
    // Mapping from instance, state, element index to denotations
    std::unordered_map<std::array<int, 3>, int> m_n_denots_mapping_per_state;
    std::unordered_map<std::array<int, 3>, bool> m_b_denots_mapping_per_state;
    std::unordered_map<std::array<int, 3>, ConceptDenotation*> m_c_denots_mapping_per_state;
    std::unordered_map<std::array<int, 3>, RoleDenotation*> m_r_denots_mapping_per_state;
    // Mapping from instance, element index to denotations
    std::unordered_map<std::array<int, 2>, int> m_n_denots_mapping_per_instance;
    std::unordered_map<std::array<int, 2>, bool> m_b_denots_mapping_per_instance;
    std::unordered_map<std::array<int, 2>, ConceptDenotation*> m_c_denots_mapping_per_instance;
    std::unordered_map<std::array<int, 2>, RoleDenotation*> m_r_denots_mapping_per_instance;
};


class Constant {
private:
    std::string m_name;
    int m_index;

    Constant(const std::string& name, int index);
    friend class VocabularyInfo;

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
};


/**
 * A Predicate belongs to a specific vocabulary of a planning domain.
 */
class Predicate {
private:
    std::string m_name;
    int m_index;
    int m_arity;
    bool m_is_static;

    Predicate(const std::string& name, int index, int arity, bool is_static=false);
    friend class VocabularyInfo;

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
    bool is_static() const;
};


/**
 * An Object belongs to a specific instance.
 */
class Object {
private:
    std::string m_name;
    int m_index;

    Object(const std::string& name, int index);
    friend class InstanceInfo;

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
};


/**
 * An Atom belongs to a specific instance.
 */
class Atom {
private:
    std::string m_name;
    int m_index;
    int m_predicate_index;
    std::vector<int> m_object_indices;
    bool m_is_static;

    Atom(const std::string& name,
        int index,
        int predicate_index,
        const std::vector<int> &object_indices,
        bool is_static=false);
    friend class InstanceInfo;

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
    const std::string& get_name() const;
    int get_index() const;
    int get_predicate_index() const;
    const std::vector<int>& get_object_indices() const;
    bool is_static() const;
};


class State {
private:
    std::shared_ptr<const InstanceInfo> m_instance_info;
    Index_Vec m_atom_indices;
    int m_index;

public:
    State(std::shared_ptr<const InstanceInfo> instance_info, const std::vector<Atom>& atoms, int index=-1);
    State(std::shared_ptr<const InstanceInfo> instance_info, const Index_Vec& atom_indices, int index=-1);
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
     * Compute a 64-Bit hash value.
     */
    size_t hash() const;

    /**
     * Setters.
     */
    void set_index(int index);

    /**
     * Getters.
     */
    std::shared_ptr<const InstanceInfo> get_instance_info() const;
    const Index_Vec& get_atom_indices() const;
    int get_index() const;
};


/**
 * VocabularyInfo stores information related to the planning domain.
 */
class VocabularyInfo {
private:
    // we store static and dynamic predicates together.
    std::unordered_map<std::string, int> m_predicate_name_to_index;
    std::vector<Predicate> m_predicates;

    std::unordered_map<std::string, int> m_constant_name_to_index;
    std::vector<Constant> m_constants;

public:
    VocabularyInfo();
    VocabularyInfo(const VocabularyInfo& other);
    VocabularyInfo& operator=(const VocabularyInfo& other);
    VocabularyInfo(VocabularyInfo&& other);
    VocabularyInfo& operator=(VocabularyInfo&& other);
    ~VocabularyInfo();

    const Predicate& add_predicate(const std::string &name, int arity, bool is_static=false);
    const Constant& add_constant(const std::string& name);

    const std::vector<Predicate>& get_predicates() const;
    const std::vector<Constant>& get_constants() const;

    // needed for parsing.
    const Predicate& get_predicate(const std::string& name) const;
    const Constant& get_constant(const std::string& name) const;
};


/**
 * InstanceInfo stores information related to the planning instance.
 */
class InstanceInfo {
private:
    std::shared_ptr<const VocabularyInfo> m_vocabulary_info;
    int m_index;

    std::unordered_map<std::string, int> m_atom_name_to_index;
    std::vector<Atom> m_atoms;

    std::unordered_map<std::string, int> m_static_atom_name_to_index;
    std::vector<Atom> m_static_atoms;

    std::unordered_map<std::string, int> m_object_name_to_index;
    std::vector<Object> m_objects;

    const Atom& add_atom(int predicate_index, const Index_Vec& object_indices, bool is_static);
    const Atom& add_atom(const Predicate& predicate, const std::vector<Object>& objects, bool is_static);
    const Atom& add_atom(const std::string& predicate_name, const std::vector<std::string>& object_names, bool is_static);

public:
    InstanceInfo() = delete;
    InstanceInfo(std::shared_ptr<const VocabularyInfo> vocabulary_info, int index=-1);
    InstanceInfo(const InstanceInfo& other);
    InstanceInfo& operator=(const InstanceInfo& other);
    InstanceInfo(InstanceInfo&& other);
    InstanceInfo& operator=(InstanceInfo&& other);
    ~InstanceInfo();

    const Object& add_object(const std::string& object_name);

    /**
     * Alternative 1 to add atoms.
     */
    const Atom& add_atom(const Predicate& predicate, const std::vector<Object>& objects);
    const Atom& add_static_atom(const Predicate& predicate, const std::vector<Object>& objects);
    /**
     * Alternative 2 to add atoms.
     */
    const Atom& add_atom(int predicate_index, const std::vector<int>& object_indices);
    const Atom& add_static_atom(int predicate_index, const std::vector<int>& object_indices);
    /**
     * Alternative 3 to add atoms.
     */
    const Atom& add_atom(const std::string& predicate_name, const std::vector<std::string>& object_names);
    const Atom& add_static_atom(const std::string& predicate_name, const std::vector<std::string>& object_names);

    /**
     * Setters.
     */
    void set_index(int index);

    /**
     * Getters.
     */
    std::shared_ptr<const VocabularyInfo> get_vocabulary_info() const;
    int get_index() const;
    const std::vector<Atom>& get_atoms() const;
    const std::vector<Atom>& get_static_atoms() const;
    const std::vector<Object>& get_objects() const;
    // convenience functions.
    const Atom& get_atom(const std::string& name) const;
    const Object& get_object(const std::string& name) const;
};


class BaseElement : public utils::Cachable {
protected:
    std::shared_ptr<const VocabularyInfo> m_vocabulary_info;

    int m_index;

protected:
    explicit BaseElement(std::shared_ptr<const VocabularyInfo> vocabulary_info);

public:
    virtual ~BaseElement();


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
     * Setters.
     */
    void set_index(int index);

    /**
     * Getters.
     */
    int get_index() const;
    std::shared_ptr<const VocabularyInfo> get_vocabulary_info() const;
};


/**
 * Concept evaluates to ConceptDenotation.
 */
class Concept : public BaseElement {
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

    ConceptDenotation evaluate(const State& state) const;
    ConceptDenotation* evaluate(const State& state, DenotationsCaches& caches) const;
    ConceptDenotations* evaluate(const States& states, DenotationsCaches& caches) const;

    int compute_complexity() const override;

    std::string compute_repr() const override;

    std::shared_ptr<const element::Concept> get_element() const;
};


/**
 * Concept evaluates to RoleDenotation.
 */
class Role : public BaseElement {
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

    RoleDenotation evaluate(const State& state) const;
    RoleDenotation* evaluate(const State& state, DenotationsCaches& caches) const;
    RoleDenotations* evaluate(const States& states, DenotationsCaches& caches) const;

    int compute_complexity() const override;

    std::string compute_repr() const override;

    std::shared_ptr<const element::Role> get_element() const;
};


/**
 * Numerical evaluates to int.
 */
class Numerical : public BaseElement {
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

    int evaluate(const State& state) const;
    int evaluate(const State& state, DenotationsCaches& caches) const;
    NumericalDenotations* evaluate(const States& states, DenotationsCaches& caches) const;

    int compute_complexity() const override;

    std::string compute_repr() const override;

    std::shared_ptr<const element::Numerical> get_element() const;
};


/**
 * Boolean evaluates to bool.
 */
class Boolean : public BaseElement {
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

    bool evaluate(const State& state) const;
    bool evaluate(const State& state, DenotationsCaches& caches) const;
    BooleanDenotations* evaluate(const States& states, DenotationsCaches& caches) const;

    int compute_complexity() const override;

    std::string compute_repr() const override;

    std::shared_ptr<const element::Boolean> get_element() const;
};


/**
 * The SyntacticElementFactory for creation of syntactically unique elements.
 */
class SyntacticElementFactory {
private:
    utils::pimpl<SyntacticElementFactoryImpl> m_pImpl;

public:
    SyntacticElementFactory(std::shared_ptr<const VocabularyInfo> vocabulary_info);
    SyntacticElementFactory(const SyntacticElementFactory& other);
    SyntacticElementFactory& operator=(const SyntacticElementFactory& other);
    SyntacticElementFactory(SyntacticElementFactory&& other);
    SyntacticElementFactory& operator=(SyntacticElementFactory&& other);
    ~SyntacticElementFactory();

    std::shared_ptr<const VocabularyInfo> get_vocabulary_info() const;

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
    Boolean make_inclusion_boolean(const Concept& concept_left, const Concept& concept_right);
    Boolean make_inclusion_boolean(const Role& role_left, const Role& role_right);
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

    Numerical make_concept_distance_numerical(const Concept& concept_from, const Role& role, const Concept& concept_to);
    Numerical make_count_numerical(const Concept& concept);
    Numerical make_count_numerical(const Role& role);
    Numerical make_role_distance_numerical(const Role& role_from, const Role& role, const Role& role_to);
    Numerical make_sum_concept_distance_numerical(const Concept& concept_from, const Role& role, const Concept& concept_to);
    Numerical make_sum_role_distance_numerical(const Role& role_from, const Role& role, const Role& role_to);

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

#endif
