/// @brief Provides functionality for the construction an evaluation of
///        domain-general state features based on description logics.

#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_H_

#include "phmap/phmap.h"
#include "utils/pimpl.h"
#include "utils/dynamic_bitset.h"

#include <memory>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <iostream>

namespace dlplan::core {
class ConceptDenotation;
class RoleDenotation;
class Constant;
class Predicate;
class VocabularyInfo;
class Object;
class Atom;
class InstanceInfo;
class State;
class SyntacticElementFactory;
class SyntacticElementFactoryImpl;
}

// Forward declare the serialize function template in boost::serialization namespace
namespace boost::serialization {
    template <typename Archive>
    void serialize(Archive& ar, dlplan::core::Constant& constant, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::core::Predicate& predicate, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::core::Object& object, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::core::Atom& atom, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::core::State& state, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::core::VocabularyInfo& vocabulary_info, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::core::InstanceInfo& instance_info, const unsigned int version);
}

namespace dlplan::core {
using ConceptDenotations = std::vector<const ConceptDenotation*>;
using RoleDenotations = std::vector<const RoleDenotation*>;
using BooleanDenotations = std::vector<bool>;
using NumericalDenotations = std::vector<int>;

using States = std::vector<State>;

using ConstantIndex = int;

using PredicateIndex = int;

using ObjectIndex = int;
using ObjectIndices = std::vector<ObjectIndex>;
using PairOfObjectIndices = std::pair<ObjectIndex, ObjectIndex>;
using PairsOfObjectIndices = std::vector<PairOfObjectIndices>;

using AtomIndex = int;
using AtomIndices = std::vector<AtomIndex>;

using ElementIndex = int;

using InstanceIndex = int;

using StateIndex = int;


template<typename T>
struct hash_impl {
    std::size_t operator()(const T&) const {
        throw std::runtime_error("hash::operator() - not implemented.");
    }
};
template<typename T>
struct hash {
    std::size_t operator()(const T& value) const {
        return hash_impl<typename std::remove_const<T>::type>()(value);
    }
};
template<>
struct hash_impl<State> {
    size_t operator()(const State& state) const;
};
template<>
struct hash_impl<ConceptDenotation> {
    size_t operator()(const ConceptDenotation& denotation) const;
};
template<>
struct hash_impl<RoleDenotation> {
    size_t operator()(const RoleDenotation& denotation) const;
};
template<>
struct hash_impl<ConceptDenotations> {
    size_t operator()(const ConceptDenotations& denotations) const;
};
template<>
struct hash_impl<RoleDenotations> {
    size_t operator()(const RoleDenotations& denotations) const;
};
template<>
struct hash_impl<bool> {
    size_t operator()(const bool& value) const;
};
template<>
struct hash_impl<int> {
    size_t operator()(const int& value) const;
};
template<>
struct hash_impl<std::vector<bool>> {
    size_t operator()(const std::vector<bool>& data) const;
};
template<>
struct hash_impl<std::vector<unsigned>> {
    size_t operator()(const std::vector<unsigned>& data) const;
};
template<>
struct hash_impl<std::vector<int>> {
    size_t operator()(const std::vector<int>& data) const;
};


/// @brief Encapsulates the result of the evaluation of a concept on a state
///        and provides functionality to access and modify it.
///
/// The result of an evaluation of a concept is a set of object indices. The
/// set of object indices represent the elements in the unary relation of the
/// concept that are true in a given state. Each object index refers to an
/// object of a common instance info.
class ConceptDenotation {
private:
    int m_num_objects;
    dlplan::utils::DynamicBitset<unsigned> m_data;

public:
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

    bool contains(ObjectIndex value) const;
    void set();
    void insert(ObjectIndex value);
    void erase(ObjectIndex value);

    int size() const;
    bool empty() const;
    bool intersects(const ConceptDenotation& other) const;
    bool is_subset_of(const ConceptDenotation& other) const;

    /// @brief Compute the canonical string representation of this concept denotation.
    /// @return The canonical string representation of this concept denotation.
    std::string compute_repr() const;

    /// @brief Overload of the output stream insertion operator (operator<<) for the ConceptDenotation class.
    ///        Outputs a string representation of a ConceptDenotation object to the specified output stream.
    /// @param os The output stream to write the string representation to.
    /// @param denotation The ConceptDenotation to be represented as a string.
    /// @return A reference to the output stream after writing the string representation.
    friend std::ostream& operator<<(std::ostream& os, const ConceptDenotation& denotation);

    /// @brief Compute a string representation of this concept denotation.
    /// @return A string representation of this concept denotation.
    std::string str() const;

    /// @brief Compute a vector representation of this concept denotation.
    /// @return A vector of object indices.
    ObjectIndices to_vector() const;

    /// @brief Compute a sorted vector representation of this concept denotation.
    /// @return A vector of object indices in ascending order.
    ObjectIndices to_sorted_vector() const;

    std::size_t hash() const;
    int get_num_objects() const;
};


/// @brief Encapsulates the result of the evaluation of a role on a state
///        and provides functionality to access and modify it.
///
/// The result of an evaluation of a role is a set of pairs of object indices.
/// The set of pairs of object indices represent the elements in the binary
/// relation of the role that are true in a given state. Each object index
/// refers to an object of a common instance info.
class RoleDenotation {
private:
    int m_num_objects;
    dlplan::utils::DynamicBitset<unsigned> m_data;

public:
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

    bool contains(const PairOfObjectIndices& value) const;
    void set();
    void insert(const PairOfObjectIndices& value);
    void erase(const PairOfObjectIndices& value);

    int size() const;
    bool empty() const;
    bool intersects(const RoleDenotation& other) const;
    bool is_subset_of(const RoleDenotation& other) const;

    /// @brief Compute the canonical string representation of this role denotation.
    /// @return The canonical string representation of this role denotation.
    std::string compute_repr() const;

    /// @brief Overload of the output stream insertion operator (operator<<) for the RoleDenotation class.
    ///        Outputs a string representation of a RoleDenotation object to the specified output stream.
    /// @param os The output stream to write the string representation to.
    /// @param denotation The RoleDenotation to be represented as a string.
    /// @return A reference to the output stream after writing the string representation.
    friend std::ostream& operator<<(std::ostream& os, const RoleDenotation& denotation);

    /// @brief Compute a string representation of this role denotation.
    /// @return A string representation of this role denotation.
    std::string str() const;

    /// @brief Compute a vector representation of this role denotation.
    /// @return A vector of pairs of object indices.
    PairsOfObjectIndices to_vector() const;

    /// @brief Compute a sorted vector representation of this role denotation.
    /// @return A vector of pairs of object indices in ascending order by first then second element.
    PairsOfObjectIndices to_sorted_vector() const;

    std::size_t hash() const;
    int get_num_objects() const;
};


/// @brief Encapsulates caches for denotations and provides functionality to
///        insert and retrieve denotations into and respectively from the cache.
class DenotationsCaches {
private:
    struct Key {
        ElementIndex element;
        InstanceIndex instance;
        StateIndex state;

        bool operator==(const Key& other) const;
        bool operator!=(const Key& other) const;
    };

    struct KeyHash  {
        std::size_t operator()(const Key& key) const;
    };

    template<typename T>
    struct Cache {
        struct UniquePtrHash {
            std::size_t operator()(const std::unique_ptr<const T>& ptr) const {
                return dlplan::core::hash<T>()(*ptr);
            }
        };

        struct UniquePtrEqual {
            bool operator()(const std::unique_ptr<const T>& left, const std::unique_ptr<const T>& right) const {
                return *left == *right;
            }
        };

        // We use unique_ptr such that other raw pointers do not become invalid.
        std::unordered_set<std::unique_ptr<const T>, UniquePtrHash, UniquePtrEqual> m_uniqueness;
        std::unordered_map<Key, const T*, KeyHash> m_per_element_instance_state_mapping;

        /// @brief Inserts denotation uniquely and returns it raw pointer.
        /// @param denotation
        /// @return
        const T* insert_denotation(T&& denotation) {
            return m_uniqueness.insert(std::make_unique<T>(std::move(denotation))).first->get();
        }

        /// @brief Inserts raw pointer of denotation into mapping from element, instance, and state.
        /// @param element_index
        /// @param instance_index
        /// @param state_index
        /// @param denotation
        void insert_denotation(ElementIndex element, InstanceIndex instance, StateIndex state, const T* denotation) {
            Key key{element, instance, state};
            m_per_element_instance_state_mapping.emplace(key, denotation);
        }

        const T* get_denotation(ElementIndex element, InstanceIndex instance, StateIndex state) const {
            Key key{element, instance, state};
            auto iter = m_per_element_instance_state_mapping.find(key);
            if (iter != m_per_element_instance_state_mapping.end()) {
                return iter->second;
            }
            return nullptr;
        }
    };

    /// @brief Cache single denotations
    Cache<ConceptDenotation> m_concept_denotation_cache;
    Cache<RoleDenotation> m_role_denotation_cache;
    Cache<bool> m_boolean_denotation_cache;
    Cache<int> m_numerical_denotation_cache;

    /// @brief Cache collection of denotations
    Cache<ConceptDenotations> m_concept_denotations_cache;
    Cache<RoleDenotations> m_role_denotations_cache;
    Cache<BooleanDenotations> m_boolean_denotations_cache;
    Cache<NumericalDenotations> m_numerical_denotations_cache;

    DenotationsCaches(const DenotationsCaches& other) = delete;
    DenotationsCaches& operator=(const DenotationsCaches& other) = delete;

public:
    DenotationsCaches();
    ~DenotationsCaches();
    DenotationsCaches(DenotationsCaches&& other);
    DenotationsCaches& operator=(DenotationsCaches&& other);

    Cache<ConceptDenotation>& get_concept_denotation_cache();
    Cache<RoleDenotation>& get_role_denotation_cache();
    Cache<bool>& get_boolean_denotation_cache();
    Cache<int>& get_numerical_denotation_cache();

    Cache<ConceptDenotations>& get_concept_denotations_cache();
    Cache<RoleDenotations>& get_role_denotations_cache();
    Cache<BooleanDenotations>& get_boolean_denotations_cache();
    Cache<NumericalDenotations>& get_numerical_denotations_cache();
};


/// @brief Encapsulates the representation of a constant and provides
///        functionality to access it.
///
/// A constant allows us to directly refer to a specfic object during the
/// evaluation of elements. While the goal of the elements is to generalize to
/// arbitrary-sized problem instances, we cannot refer to all objects directly.
/// However, we want to be able to refer to some special objects directly. For
/// example, consider that an instance from a common planning domain contains a
/// set of natural numbers from 0 to n. Declaring 0 as constant allows us to
/// define a concept `c_one_of(0)` that always evaluates to 0 for any given
/// state.
class Constant {
private:
    ///< The name of the constant.
    std::string m_name;
    ///< The index of the constant.
    ConstantIndex m_index;

    Constant(const std::string& name, ConstantIndex index);
    friend class VocabularyInfo;

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, Constant& constant, const unsigned int version);

public:
    Constant();
    Constant(const Constant& other);
    Constant& operator=(const Constant& other);
    Constant(Constant&& other);
    Constant& operator=(Constant&& other);
    ~Constant();

    /// @brief Compute the canonical string representation of this constant.
    /// @return The canonical string representation of this constant.
    std::string compute_repr() const;

    /// @brief Overload of the output stream insertion operator (operator<<) for the Constant class.
    ///        Outputs a string representation of a Constant object to the specified output stream.
    /// @param os The output stream to write the string representation to.
    /// @param denotation The Constant to be represented as a string.
    /// @return A reference to the output stream after writing the string representation.
    friend std::ostream& operator<<(std::ostream& os, const Constant& constant);

    /// @brief Compute a string representation of this constant.
    /// @return A string representation of this constant.
    std::string str() const;

    /// @brief Checks if this constant is equal to another constant.
    /// @param other The constant to compare against.
    /// @return True if the constants are equal, false otherwise.
    bool operator==(const Constant& other) const;

    /// @brief Checks if this constant is not equal to another constant.
    /// @param other The constant to compare against.
    /// @return True if the constants are not equal, false otherwise.
    bool operator!=(const Constant& other) const;

    /// @brief Retrieves the index of the constant.
    /// @return The index of the constant.
    ConstantIndex get_index() const;

    /// @brief Retrieves the name of the constant.
    /// @return The name of the constant.
    const std::string& get_name() const;
};


/// @brief Encapsulates the representation of a relation and provides
///        functionality to access it.
///
/// Planning domains define a set of predicates. Each predicate is a name for
/// a relation with variable-sized arity. A predicate can also be defined as
/// static with meaning that the ground atoms in all instances over the
/// the common planning domain will also be static.
class Predicate {
private:
    std::string m_name;
    PredicateIndex m_index;
    int m_arity;
    bool m_is_static;

    Predicate(const std::string& name, PredicateIndex index, int arity, bool is_static=false);
    friend class VocabularyInfo;

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, Predicate& predicate, const unsigned int version);


public:
    Predicate();
    Predicate(const Predicate& other);
    Predicate& operator=(const Predicate& other);
    Predicate(Predicate&& other);
    Predicate& operator=(Predicate&& other);
    ~Predicate();

    bool operator==(const Predicate& other) const;
    bool operator!=(const Predicate& other) const;

    /// @brief Compute the canonical string representation of this predicate.
    /// @return The canonical string representation of this predicate.
    std::string compute_repr() const;

    /// @brief Overload of the output stream insertion operator (operator<<) for the Predicate class.
    ///        Outputs a string representation of a Predicate object to the specified output stream.
    /// @param os The output stream to write the string representation to.
    /// @param denotation The Predicate to be represented as a string.
    /// @return A reference to the output stream after writing the string representation.
    friend std::ostream& operator<<(std::ostream& os, const Predicate& predicate);

    /// @brief Compute a string representation of this predicate.
    /// @return A string representation of this predicate.
    std::string str() const;

    PredicateIndex get_index() const;
    const std::string& get_name() const;
    int get_arity() const;
    bool is_static() const;
};


/// @brief Encapsulates domain-general data and provides functionality
///        to access it.
///
/// A vocabulary info defines information related to the predicates and
/// and constants of a planning domain. We usually also want to add
/// goal versions of predicates with an additional suffix _g. This allows us
/// to add static atoms for the atoms that are true in the goal and refer to
/// the goal during the evaluation of elements.
class VocabularyInfo {
private:
    // we store static and dynamic predicates together.
    std::unordered_map<std::string, PredicateIndex> m_predicate_name_to_index;
    std::vector<Predicate> m_predicates;

    std::unordered_map<std::string, ConstantIndex> m_constant_name_to_index;
    std::vector<Constant> m_constants;

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, VocabularyInfo& vocabulary_info, const unsigned int version);

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

    const Predicate& get_predicate(const std::string& name) const;
    const Constant& get_constant(const std::string& name) const;

    /// @brief Compute the canonical string representation of this vocabulary.
    /// @return The canonical string representation of this vocabulary.
    std::string compute_repr() const;

    /// @brief Overload of the output stream insertion operator (operator<<) for the VocabularyInfo class.
    ///        Outputs a string representation of a VocabularyInfo object to the specified output stream.
    /// @param os The output stream to write the string representation to.
    /// @param denotation The VocabularyInfo to be represented as a string.
    /// @return A reference to the output stream after writing the string representation.
    friend std::ostream& operator<<(std::ostream& os, const VocabularyInfo& vocabulary);

    /// @brief Compute a string representation of this vocabulary.
    /// @return A string representation of this vocabulary.
    std::string str() const;
};


/// @brief Encapsulates the representation of an object and provides
///        functionality to access it.
class Object {
private:
    std::string m_name;
    ObjectIndex m_index;

    Object(const std::string& name, int index);
    friend class InstanceInfo;

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, Object& object, const unsigned int version);

public:
    Object();
    Object(const Object& other);
    Object& operator=(const Object& other);
    Object(Object&& other);
    Object& operator=(Object&& other);
    ~Object();

    bool operator==(const Object& other) const;
    bool operator!=(const Object& other) const;

    /// @brief Compute the canonical string representation of this object.
    /// @return The canonical string representation of this object.
    std::string compute_repr() const;

    /// @brief Overload of the output stream insertion operator (operator<<) for the Object class.
    ///        Outputs a string representation of a Object object to the specified output stream.
    /// @param os The output stream to write the string representation to.
    /// @param denotation The Object to be represented as a string.
    /// @return A reference to the output stream after writing the string representation.
    friend std::ostream& operator<<(std::ostream& os, const Object& object);

    /// @brief Compute a string representation of this object.
    /// @return A string representation of this object.
    std::string str() const;

    ObjectIndex get_index() const;
    const std::string& get_name() const;
};


/// @brief Encapsulates the representation of an element in the relation
///        of a predicate and provides functionality to access it.
class Atom {
private:
    std::string m_name;
    AtomIndex m_index;
    PredicateIndex m_predicate_index;
    ObjectIndices m_object_indices;
    bool m_is_static;

    Atom(const std::string& name,
        AtomIndex index,
        PredicateIndex predicate_index,
        const ObjectIndices &object_indices,
        bool is_static=false);
    friend class InstanceInfo;

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, Atom& atom, const unsigned int version);


public:
    Atom();
    Atom(const Atom& other);
    Atom& operator=(const Atom& other);
    Atom(Atom&& other);
    Atom& operator=(Atom&& other);
    ~Atom();

    bool operator==(const Atom& other) const;
    bool operator!=(const Atom& other) const;

    /// @brief Compute the canonical string representation of this atom.
    /// @return The canonical string representation of this atom.
    std::string compute_repr() const;

    /// @brief Overload of the output stream insertion operator (operator<<) for the Atom class.
    ///        Outputs a string representation of a Atom object to the specified output stream.
    /// @param os The output stream to write the string representation to.
    /// @param denotation The Atom to be represented as a string.
    /// @return A reference to the output stream after writing the string representation.
    friend std::ostream& operator<<(std::ostream& os, const Atom& atom);

    /// @brief Compute a string representation of this atom.
    /// @return A string representation of this atom.
    std::string str() const;

    const std::string& get_name() const;
    AtomIndex get_index() const;
    PredicateIndex get_predicate_index() const;
    const ObjectIndices& get_object_indices() const;
    bool is_static() const;
};


/// @brief Encapsulates instance specific data and provides functionality
///        to access it.
class InstanceInfo {
private:
    std::shared_ptr<const VocabularyInfo> m_vocabulary_info;
    InstanceIndex m_index;

    std::unordered_map<std::string, AtomIndex> m_atom_name_to_index;
    std::vector<Atom> m_atoms;

    std::unordered_map<std::string, AtomIndex> m_static_atom_name_to_index;
    std::vector<Atom> m_static_atoms;

    std::unordered_map<std::string, ObjectIndex> m_object_name_to_index;
    std::vector<Object> m_objects;

    const Atom& add_atom(PredicateIndex predicate_index, const ObjectIndices& object_indices, bool is_static);
    const Atom& add_atom(const Predicate& predicate, const std::vector<Object>& objects, bool is_static);
    const Atom& add_atom(const std::string& predicate_name, const std::vector<std::string>& object_names, bool is_static);

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, InstanceInfo& instance_info, const unsigned int version);

public:
    InstanceInfo();
    InstanceInfo(std::shared_ptr<const VocabularyInfo> vocabulary_info, InstanceIndex index=-1);
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
    const Atom& add_atom(PredicateIndex predicate_index, const ObjectIndices& object_indices);
    const Atom& add_static_atom(PredicateIndex predicate_index, const ObjectIndices& object_indices);
    /**
     * Alternative 3 to add atoms.
     */
    const Atom& add_atom(const std::string& predicate_name, const std::vector<std::string>& object_names);
    const Atom& add_static_atom(const std::string& predicate_name, const std::vector<std::string>& object_names);

    /// @brief Compute the canonical string representation of this instance.
    /// @return The canonical string representation of this instance.
    ///
    /// @note This representation does not include the recursive conversion of shared resources.
    ///       Users are responsible for handling shared resources separately if desired.
    std::string compute_repr() const;

    /// @brief Overload of the output stream insertion operator (operator<<) for the InstanceInfo class.
    ///        Outputs a string representation of a InstanceInfo object to the specified output stream.
    /// @param os The output stream to write the string representation to.
    /// @param denotation The InstanceInfo to be represented as a string.
    /// @return A reference to the output stream after writing the string representation.
    ///
    /// @note This representation does not include the recursive conversion of shared resources.
    ///       Users are responsible for handling shared resources separately if desired.
    friend std::ostream& operator<<(std::ostream& os, const InstanceInfo& instance);

    /// @brief Compute a string representation of this instance.
    /// @return A string representation of this instance.
    std::string str() const;

    void set_index(InstanceIndex index);
    std::shared_ptr<const VocabularyInfo> get_vocabulary_info() const;
    InstanceIndex get_index() const;
    const std::vector<Atom>& get_atoms() const;
    const std::vector<Atom>& get_static_atoms() const;
    const std::vector<Object>& get_objects() const;
    const Atom& get_atom(const std::string& name) const;
    const Object& get_object(const std::string& name) const;
};


/// @brief Encapsulates the atoms that are considered to be true in the
///        current situation and provides functionality to access it.
class State {
private:
    std::shared_ptr<const InstanceInfo> m_instance_info;
    AtomIndices m_atom_indices;
    int m_index;

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, State& state, const unsigned int version);


public:
    State();
    State(std::shared_ptr<const InstanceInfo> instance_info, const std::vector<Atom>& atoms, StateIndex index=-1);
    State(std::shared_ptr<const InstanceInfo> instance_info, const AtomIndices& atom_indices, StateIndex index=-1);
    State(std::shared_ptr<const InstanceInfo> instance_info, AtomIndices&& atom_indices, StateIndex index=-1);
    State(const State& other);
    State& operator=(const State& other);
    State(State&& other);
    State& operator=(State&& other);
    ~State();

    bool operator==(const State& other) const;
    bool operator!=(const State& other) const;

    /// @brief Compute the canonical string representation of this state.
    /// @return The canonical string representation of this state.
    ///
    /// @note This representation does not include the recursive conversion of shared resources.
    ///       Users are responsible for handling shared resources separately if desired.
    std::string compute_repr() const;

    /// @brief Overload of the output stream insertion operator (operator<<) for the State class.
    ///        Outputs a string representation of a State object to the specified output stream.
    /// @param os The output stream to write the string representation to.
    /// @param denotation The State to be represented as a string.
    /// @return A reference to the output stream after writing the string representation.
    ///
    /// @note This representation does not include the recursive conversion of shared resources.
    ///       Users are responsible for handling shared resources separately if desired.
    friend std::ostream& operator<<(std::ostream& os, const State& state);

    /// @brief Compute a string representation of this state.
    /// @return A string representation of this state.
    std::string str() const;

    size_t hash() const;
    void set_index(StateIndex index);
    std::shared_ptr<const InstanceInfo> get_instance_info() const;
    const AtomIndices& get_atom_indices() const;
    StateIndex get_index() const;
};


/// @brief Represents the abstract base class of an element with functionality
///        for computing string representations and its complexity.
class BaseElement {
protected:
    std::shared_ptr<const VocabularyInfo> m_vocabulary_info;

    ElementIndex m_index;
    /**
     * if true then element is evaluated per instance rather than per state.
     */
    bool m_is_static;

protected:
    explicit BaseElement(std::shared_ptr<const VocabularyInfo> vocabulary_info, bool is_static);

public:
    virtual ~BaseElement();

    /**
     * Returns the complexity of the element
     * measured in the size of the abstract syntax tree.
     */
    virtual int compute_complexity() const = 0;

    /// @brief Compute the canonical string representation of this element.
    /// @return The canonical string representation of this element.
    std::string compute_repr() const;
    virtual void compute_repr(std::stringstream& out) const = 0;

    /// @brief Computes a time score for evaluating this element relative to other elements.
    ///        The scoring assumes evaluation that uses caching.
    /// @return An integer that represents the score.
    virtual int compute_evaluate_time_score() const = 0;

    /// @brief Overload of the output stream insertion operator (operator<<) for the BaseElement class.
    ///        Outputs a string representation of a BaseElement object to the specified output stream.
    /// @param os The output stream to write the string representation to.
    /// @param denotation The BaseElement to be represented as a string.
    /// @return A reference to the output stream after writing the string representation.
    friend std::ostream& operator<<(std::ostream& os, const BaseElement& element);

    /// @brief Compute a string representation of this element.
    /// @return A string representation of this element.
    std::string str() const;

    void set_index(ElementIndex index);
    ElementIndex get_index() const;
    std::shared_ptr<const VocabularyInfo> get_vocabulary_info() const;
    bool is_static() const;
};


/// @brief Represents a concept element that evaluates to a concept denotation
///        on a given state. It can also make use of a cache during evaluation.
class Concept : public BaseElement {
protected:
    Concept(std::shared_ptr<const VocabularyInfo> vocabulary_info, bool is_static);
    friend class SyntacticElementFactoryImpl;

    virtual ConceptDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const = 0;
    virtual ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const = 0;

public:
    Concept(const Concept& other);
    Concept& operator=(const Concept& other);
    Concept(Concept&& other);
    Concept& operator=(Concept&& other);
    ~Concept() override;

    virtual ConceptDenotation evaluate(const State& state) const = 0;
    const ConceptDenotation* evaluate(const State& state, DenotationsCaches& caches) const;
    const ConceptDenotations* evaluate(const States& states, DenotationsCaches& caches) const;
};


/// @brief Represents a role element that evaluates to a role denotation
///        on a given state. It can also make use of a cache during evaluation.
class Role : public BaseElement {
protected:
    Role(std::shared_ptr<const VocabularyInfo> vocabulary_info, bool is_static);
    friend class SyntacticElementFactoryImpl;

    virtual RoleDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const = 0;
    virtual RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const = 0;

public:
    Role(const Role& other);
    Role& operator=(const Role& other);
    Role(Role&& other);
    Role& operator=(Role&& other);
    ~Role() override;

    virtual RoleDenotation evaluate(const State& state) const = 0;
    const RoleDenotation* evaluate(const State& state, DenotationsCaches& caches) const;
    const RoleDenotations* evaluate(const States& states, DenotationsCaches& caches) const;
};


/// @brief Represents a numerical element that evaluates to an natural number
///        on a given state. It can also make use of a cache during evaluation.
class Numerical : public BaseElement {
protected:
    Numerical(std::shared_ptr<const VocabularyInfo> vocabulary_info, bool is_static);
    friend class SyntacticElementFactoryImpl;

    virtual int evaluate_impl(const State& state, DenotationsCaches& caches) const = 0;
    virtual NumericalDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const = 0;

public:
    Numerical(const Numerical& other);
    Numerical& operator=(const Numerical& other);
    Numerical(Numerical&& other);
    Numerical& operator=(Numerical&& other);
    ~Numerical() override;

    virtual int evaluate(const State& state) const = 0;
    int evaluate(const State& state, DenotationsCaches& caches) const;
    const NumericalDenotations* evaluate(const States& states, DenotationsCaches& caches) const;
};


/// @brief Represents a Boolean element that evaluates to either true or false
///        on a given state. It can also make use of a cache during evaluation.
class Boolean : public BaseElement {
protected:
    Boolean(std::shared_ptr<const VocabularyInfo> vocabulary_info, bool is_static);
    friend class SyntacticElementFactoryImpl;

    virtual bool evaluate_impl(const State& state, DenotationsCaches& caches) const = 0;
    virtual BooleanDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const = 0;

public:
    Boolean(const Boolean& other);
    Boolean& operator=(const Boolean& other);
    Boolean(Boolean&& other);
    Boolean& operator=(Boolean&& other);
    ~Boolean() override;

    virtual bool evaluate(const State& state) const = 0;
    bool evaluate(const State& state, DenotationsCaches& caches) const;
    const BooleanDenotations* evaluate(const States& states, DenotationsCaches& caches) const;
};


/// @brief Provides functionality for the syntactically unique creation of elements.
class SyntacticElementFactory {
private:
    dlplan::utils::pimpl<SyntacticElementFactoryImpl> m_pImpl;

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
    std::shared_ptr<const Concept> parse_concept(const std::string &description);

    /**
     * Returns a Role if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    std::shared_ptr<const Role> parse_role(const std::string &description);

    /**
     * Returns a Numerical if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    std::shared_ptr<const Numerical> parse_numerical(const std::string &description);

    /**
     * Returns a Boolean if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    std::shared_ptr<const Boolean> parse_boolean(const std::string &description);

    std::shared_ptr<const Boolean> make_empty_boolean(const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Boolean> make_empty_boolean(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Boolean> make_inclusion_boolean(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Boolean> make_inclusion_boolean(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Boolean> make_nullary_boolean(const Predicate& predicate);

    std::shared_ptr<const Concept> make_all_concept(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Concept> make_and_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Concept> make_bot_concept();
    std::shared_ptr<const Concept> make_diff_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Concept> make_equal_concept(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Concept> make_not_concept(const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Concept> make_one_of_concept(const Constant& constant);
    std::shared_ptr<const Concept> make_or_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Concept> make_projection_concept(const std::shared_ptr<const Role>& role, int pos);
    std::shared_ptr<const Concept> make_primitive_concept(const Predicate& predicate, int pos);
    std::shared_ptr<const Concept> make_some_concept(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Concept> make_subset_concept(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Concept> make_top_concept();

    std::shared_ptr<const Numerical> make_concept_distance_numerical(const std::shared_ptr<const Concept>& concept_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_to);
    std::shared_ptr<const Numerical> make_count_numerical(const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Numerical> make_count_numerical(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Numerical> make_role_distance_numerical(const std::shared_ptr<const Role>& role_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Role>& role_to);
    std::shared_ptr<const Numerical> make_sum_concept_distance_numerical(const std::shared_ptr<const Concept>& concept_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_to);
    std::shared_ptr<const Numerical> make_sum_role_distance_numerical(const std::shared_ptr<const Role>& role_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Role>& role_to);

    std::shared_ptr<const Role> make_and_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_compose_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_diff_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_identity_role(const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Role> make_inverse_role(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Role> make_not_role(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Role> make_or_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_primitive_role(const Predicate& predicate, int pos_1, int pos_2);
    std::shared_ptr<const Role> make_restrict_role(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Role> make_top_role();
    std::shared_ptr<const Role> make_transitive_closure(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Role> make_transitive_reflexive_closure(const std::shared_ptr<const Role>& role);
};

}

#endif
