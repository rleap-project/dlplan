/// @brief Provides functionality for the construction an evaluation of
///        domain-general state features based on description logics.

#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_H_

#include <memory>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "common/base.h"
#include "common/parsers/config.hpp"
#include "utils/pimpl.h"
#include "utils/dynamic_bitset.h"
#include "utils/cache.h"


// Forward declarations of this header
namespace dlplan::core {
class ConceptDenotation;
class RoleDenotation;
class DenotationsCaches;
struct DenotationsCacheKey;
class Constant;
class Predicate;
class VocabularyInfo;
class Object;
class Atom;
class InstanceInfo;
class State;
class SyntacticElementFactory;
class SyntacticElementFactoryImpl;

using ConceptDenotations = std::vector<std::shared_ptr<const ConceptDenotation>>;
using RoleDenotations = std::vector<std::shared_ptr<const RoleDenotation>>;
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
}


namespace std {
    template<>
    struct hash<dlplan::core::Constant> {
        std::size_t operator()(const dlplan::core::Constant& constant) const;
    };
    template<>
    struct hash<dlplan::core::Predicate> {
        std::size_t operator()(const dlplan::core::Predicate& predicate) const;
    };
    template<>
    struct hash<dlplan::core::Object> {
        std::size_t operator()(const dlplan::core::Object& object) const;
    };
    template<>
    struct hash<dlplan::core::Atom> {
        std::size_t operator()(const dlplan::core::Atom& atom) const;
    };
    template<>
    struct hash<dlplan::core::State> {
        size_t operator()(const dlplan::core::State& state) const;
    };
    template<>
    struct hash<dlplan::core::ConceptDenotation> {
        size_t operator()(const dlplan::core::ConceptDenotation& denotation) const;
    };
    template<>
    struct hash<dlplan::core::RoleDenotation> {
        size_t operator()(const dlplan::core::RoleDenotation& denotation) const;
    };
    template<>
    struct hash<dlplan::core::ConceptDenotations> {
        size_t operator()(const dlplan::core::ConceptDenotations& denotations) const;
    };
    template<>
    struct hash<dlplan::core::RoleDenotations> {
        size_t operator()(const dlplan::core::RoleDenotations& denotations) const;
    };
    template<>
    struct hash<dlplan::core::DenotationsCacheKey> {
        std::size_t operator()(const dlplan::core::DenotationsCacheKey& key) const;
    };
}

namespace dlplan::core {
/// @brief Encapsulates the result of the evaluation of a concept on a state
///        and provides functionality to access and modify it.
///
/// The result of an evaluation of a concept is a set of object indices. The
/// set of object indices represent the elements in the unary relation of the
/// concept that are true in a given state. Each object index refers to an
/// object of a common instance info.
class ConceptDenotation : public Base<ConceptDenotation> {
private:
    int m_num_objects;
    DynamicBitset<unsigned> m_data;

public:
    ConceptDenotation(int num_objects);
    ConceptDenotation(const ConceptDenotation& other);
    ConceptDenotation& operator=(const ConceptDenotation& other);
    ConceptDenotation(ConceptDenotation&& other);
    ConceptDenotation& operator=(ConceptDenotation&& other);
    ~ConceptDenotation();

    bool are_equal_impl(const ConceptDenotation& other) const;
    void str_impl(std::stringstream& out) const;
    std::size_t hash_impl() const;

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

    /// @brief Compute a vector representation of this concept denotation.
    /// @return A vector of object indices.
    ObjectIndices to_vector() const;

    /// @brief Compute a sorted vector representation of this concept denotation.
    /// @return A vector of object indices in ascending order.
    ObjectIndices to_sorted_vector() const;

    int get_num_objects() const;
};


/// @brief Encapsulates the result of the evaluation of a role on a state
///        and provides functionality to access and modify it.
///
/// The result of an evaluation of a role is a set of pairs of object indices.
/// The set of pairs of object indices represent the elements in the binary
/// relation of the role that are true in a given state. Each object index
/// refers to an object of a common instance info.
class RoleDenotation : public Base<RoleDenotation> {
private:
    int m_num_objects;
    DynamicBitset<unsigned> m_data;

public:
    explicit RoleDenotation(int num_objects);
    RoleDenotation(const RoleDenotation& other);
    RoleDenotation& operator=(const RoleDenotation& other);
    RoleDenotation(RoleDenotation&& other);
    RoleDenotation& operator=(RoleDenotation&& other);
    ~RoleDenotation();

    bool are_equal_impl(const RoleDenotation& other) const ;
    void str_impl(std::stringstream& out) const;
    std::size_t hash_impl() const;

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

    /// @brief Compute a vector representation of this role denotation.
    /// @return A vector of pairs of object indices.
    PairsOfObjectIndices to_vector() const;

    /// @brief Compute a sorted vector representation of this role denotation.
    /// @return A vector of pairs of object indices in ascending order by first then second element.
    PairsOfObjectIndices to_sorted_vector() const;

    int get_num_objects() const;
};

/// @brief Encapsulates a key to store and retrieve denotations from the cache.
struct DenotationsCacheKey {
    ElementIndex element;
    InstanceIndex instance;
    StateIndex state;

    bool operator==(const DenotationsCacheKey& other) const;

    size_t hash() const;
};


/// @brief Encapsulates caches for denotations and provides functionality to
///        insert and retrieve denotations into and respectively from the cache.
class DenotationsCaches {
public:

    DenotationsCaches();
    ~DenotationsCaches();
    DenotationsCaches(const DenotationsCaches& other) = delete;
    DenotationsCaches& operator=(const DenotationsCaches& other) = delete;
    DenotationsCaches(DenotationsCaches&& other);
    DenotationsCaches& operator=(DenotationsCaches&& other);

    // Caches denotations by key, same denotations are shared.
    SharedObjectCache<DenotationsCacheKey,
        ConceptDenotation,
        RoleDenotation,
        bool,
        int,
        ConceptDenotations,
        RoleDenotations,
        BooleanDenotations,
        NumericalDenotations> data;
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
class Constant : public Base<Constant> {
private:
    ///< The name of the constant.
    std::string m_name;

    Constant(ConstantIndex index, const std::string& name);

    friend class VocabularyInfo;
public:
    Constant(const Constant& other);
    Constant& operator=(const Constant& other);
    Constant(Constant&& other);
    Constant& operator=(Constant&& other);
    ~Constant();

    bool are_equal_impl(const Constant& other) const;
    void str_impl(std::stringstream& out) const;
    size_t hash_impl() const;

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
class Predicate : public Base<Predicate> {
private:
    std::string m_name;
    int m_arity;
    bool m_is_static;

    Predicate(PredicateIndex index, const std::string& name, int arity, bool is_static=false);

    friend class VocabularyInfo;

public:
    Predicate(const Predicate& other);
    Predicate& operator=(const Predicate& other);
    Predicate(Predicate&& other);
    Predicate& operator=(Predicate&& other);
    ~Predicate();

    bool are_equal_impl(const Predicate& other) const;
    void str_impl(std::stringstream& out) const;
    size_t hash_impl() const;

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
class VocabularyInfo : public Base<VocabularyInfo> {
private:
    // we store static and dynamic predicates together.
    std::unordered_map<std::string, PredicateIndex> m_predicate_name_to_index;
    std::vector<Predicate> m_predicates;

    std::unordered_map<std::string, ConstantIndex> m_constant_name_to_index;
    std::vector<Constant> m_constants;

public:
    VocabularyInfo();
    VocabularyInfo(const VocabularyInfo& other);
    VocabularyInfo& operator=(const VocabularyInfo& other);
    VocabularyInfo(VocabularyInfo&& other);
    VocabularyInfo& operator=(VocabularyInfo&& other);
    ~VocabularyInfo();

    bool are_equal_impl(const VocabularyInfo& other) const;
    void str_impl(std::stringstream& out) const;
    size_t hash_impl() const;

    const Predicate& add_predicate(const std::string &name, int arity, bool is_static=false);
    const Constant& add_constant(const std::string& name);

    const std::unordered_map<std::string, PredicateIndex>& get_predicates_mapping() const;
    const std::unordered_map<std::string, ConstantIndex>& get_constants_mapping() const;

    const std::vector<Predicate>& get_predicates() const;
    const std::vector<Constant>& get_constants() const;

    const Predicate& get_predicate(const std::string& name) const;
    const Constant& get_constant(const std::string& name) const;
};


/// @brief Encapsulates the representation of an object and provides
///        functionality to access it.
class Object : public Base<Object> {
private:
    std::string m_name;

    Object(ObjectIndex index, const std::string& name);

    friend class InstanceInfo;

public:
    Object(const Object& other);
    Object& operator=(const Object& other);
    Object(Object&& other);
    Object& operator=(Object&& other);
    ~Object();

    bool are_equal_impl(const Object& other) const;
    void str_impl(std::stringstream& out) const;
    size_t hash_impl() const;

    const std::string& get_name() const;
};


/// @brief Encapsulates the representation of an element in the relation
///        of a predicate and provides functionality to access it.
class Atom : public Base<Atom> {
private:
    std::string m_name;
    PredicateIndex m_predicate_index;
    ObjectIndices m_object_indices;
    bool m_is_static;

    Atom(AtomIndex index,
        const std::string& name,
        PredicateIndex predicate_index,
        const ObjectIndices &object_indices,
        bool is_static=false);

    friend class InstanceInfo;

public:
    Atom(const Atom& other);
    Atom& operator=(const Atom& other);
    Atom(Atom&& other);
    Atom& operator=(Atom&& other);
    ~Atom();

    bool are_equal_impl(const Atom& other) const;
    void str_impl(std::stringstream& out) const;
    size_t hash_impl() const;

    const std::string& get_name() const;
    PredicateIndex get_predicate_index() const;
    const ObjectIndices& get_object_indices() const;
    bool is_static() const;
};


/// @brief Encapsulates instance specific data and provides functionality
///        to access it.
class InstanceInfo : public Base<InstanceInfo> {
private:
    std::shared_ptr<VocabularyInfo> m_vocabulary_info;

    std::unordered_map<std::string, AtomIndex> m_atom_name_to_index;
    std::vector<Atom> m_atoms;

    std::unordered_map<std::string, AtomIndex> m_static_atom_name_to_index;
    std::vector<Atom> m_static_atoms;

    std::unordered_map<std::string, ObjectIndex> m_object_name_to_index;
    std::vector<Object> m_objects;

    const Atom& add_atom(PredicateIndex predicate_index, const ObjectIndices& object_indices, bool is_static);
    const Atom& add_atom(const Predicate& predicate, const std::vector<Object>& objects, bool is_static);
    const Atom& add_atom(const std::string& predicate_name, const std::vector<std::string>& object_names, bool is_static);

public:
    InstanceInfo(InstanceIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info);
    InstanceInfo(const InstanceInfo& other);
    InstanceInfo& operator=(const InstanceInfo& other);
    InstanceInfo(InstanceInfo&& other);
    InstanceInfo& operator=(InstanceInfo&& other);
    ~InstanceInfo();

    bool are_equal_impl(const InstanceInfo& other) const;
    void str_impl(std::stringstream& out) const;
    size_t hash_impl() const;

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

    /// @brief Removes all atoms from the instance.
    void clear_atoms();
    /// @brief Removes all static atoms from the instance.
    void clear_static_atoms();

    std::shared_ptr<VocabularyInfo> get_vocabulary_info() const;
    const std::vector<Atom>& get_atoms() const;
    const std::vector<Atom>& get_static_atoms() const;
    const std::vector<Object>& get_objects() const;
    const Atom& get_atom(const std::string& name) const;
    const Object& get_object(const std::string& name) const;
};


/// @brief Encapsulates the atoms that are considered to be true in the
///        current situation and provides functionality to access it.
class State : public Base<State> {
private:
    std::shared_ptr<InstanceInfo> m_instance_info;
    AtomIndices m_atom_indices;

public:
    State(StateIndex index, std::shared_ptr<InstanceInfo> instance_info, const std::vector<Atom>& atoms);
    State(StateIndex index, std::shared_ptr<InstanceInfo> instance_info, const AtomIndices& atom_indices);
    State(StateIndex index, std::shared_ptr<InstanceInfo> instance_info, AtomIndices&& atom_indices);
    State(const State& other);
    State& operator=(const State& other);
    State(State&& other);
    State& operator=(State&& other);
    ~State();

    bool are_equal_impl(const State& other) const;
    void str_impl(std::stringstream& out) const;
    size_t hash_impl() const;

    std::shared_ptr<InstanceInfo> get_instance_info() const;
    const AtomIndices& get_atom_indices() const;
};


/// @brief Represents the abstract base class of an element
///        with functionality for computing some metric scores.
template<typename Derived>
class BaseElement : public Base<Derived> {
protected:
    std::shared_ptr<VocabularyInfo> m_vocabulary_info;
    bool m_is_static;

public:
    BaseElement(int index, std::shared_ptr<VocabularyInfo> vocabulary_info, bool is_static)
        : Base<Derived>(index), m_vocabulary_info(vocabulary_info), m_is_static(is_static) { }

    ~BaseElement() { }

    /// @brief Returns the complexity of the element
    ///        measured in the size of the abstract syntax tree.
    int compute_complexity() const {
        return static_cast<const Derived*>(this)->compute_complexity_impl();
    }

    /// @brief Computes a time score for evaluating this element relative to other elements.
    ///        The scoring assumes evaluation that uses caching.
    int compute_evaluate_time_score() const {
        return static_cast<const Derived*>(this)->compute_evaluate_time_score_impl();
    }

    /* Getters. */
    std::shared_ptr<VocabularyInfo> get_vocabulary_info() const { return m_vocabulary_info; }
    bool is_static() const { return m_is_static; }
};



template<typename Denotation, typename DenotationList>
class Element : public BaseElement<Element<Denotation, DenotationList>> {
protected:
    Element(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, bool is_static)
       : BaseElement<Element<Denotation, DenotationList>>(index, vocabulary_info, is_static) { }

    // protected copy/move to prevent accidental object slicing when passed by value
    Element(const Element& other) = default;
    Element& operator=(const Element& other) = default;
    Element(Element&& other) = default;
    Element& operator=(Element&& other) = default;

    virtual Denotation evaluate_impl(const State& , DenotationsCaches& ) const = 0;
    virtual DenotationList evaluate_impl(const States& , DenotationsCaches& ) const = 0;

public:
    virtual ~Element() = default;

    virtual bool are_equal_impl(const Element& other) const = 0;
    virtual size_t hash_impl() const = 0;
    virtual void str_impl(std::stringstream& out) const = 0;
    virtual int compute_complexity_impl() const = 0;
    virtual int compute_evaluate_time_score_impl() const = 0;

    virtual Denotation evaluate(const State& ) const = 0;
    std::shared_ptr<const Denotation> evaluate(const State& state, DenotationsCaches& caches) const {
        auto key = DenotationsCacheKey{ Base<Element<Denotation, DenotationList>>::get_index(), state.get_instance_info()->get_index(), BaseElement<Element<Denotation, DenotationList>>::is_static() ? -1 : state.get_index() };
        auto cached = caches.data.get<Denotation>(key);
        if (cached) return cached;
        auto denotation = caches.data.insert_unique(evaluate_impl(state, caches));
        caches.data.insert_mapping(key, denotation);
        return denotation;
    }
    std::shared_ptr<const DenotationList> evaluate(const States& states, DenotationsCaches& caches) const {
        auto key = DenotationsCacheKey{ Base<Element<Denotation, DenotationList>>::get_index(), -1, -1 };
        auto cached = caches.data.get<DenotationList>(key);
        if (cached) return cached;
        auto result_denotations = caches.data.insert_unique(evaluate_impl(states, caches));
        caches.data.insert_mapping(key, result_denotations);
        return result_denotations;
    }
};

template<typename Denotation, typename DenotationList>
class ElementLight : public BaseElement<ElementLight<Denotation, DenotationList>> {
protected:
    ElementLight(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, bool is_static)
       : BaseElement<ElementLight<Denotation, DenotationList>>(index, vocabulary_info, is_static) { }

    // protected copy/move to prevent accidental object slicing when passed by value
    ElementLight(const ElementLight& other) = default;
    ElementLight& operator=(const ElementLight& other) = default;
    ElementLight(ElementLight&& other) = default;
    ElementLight& operator=(ElementLight&& other) = default;

    virtual Denotation evaluate_impl(const State& , DenotationsCaches& ) const = 0;
    virtual DenotationList evaluate_impl(const States& , DenotationsCaches& ) const = 0;

public:
    virtual ~ElementLight() = default;

    virtual bool are_equal_impl(const ElementLight& other) const = 0;
    virtual size_t hash_impl() const = 0;
    virtual void str_impl(std::stringstream& out) const = 0;
    virtual int compute_complexity_impl() const = 0;
    virtual int compute_evaluate_time_score_impl() const = 0;

    virtual Denotation evaluate(const State& ) const = 0;
    Denotation evaluate(const State& state, DenotationsCaches& caches) const {
        auto key = DenotationsCacheKey{ Base<ElementLight<Denotation, DenotationList>>::get_index(), state.get_instance_info()->get_index(), BaseElement<ElementLight<Denotation, DenotationList>>::is_static() ? -1 : state.get_index() };
        auto cached = caches.data.get<Denotation>(key);
        // ElementLight dereferences the denotation because it is cheap to copy,
        // e.g. std::shared_ptr<const int> -> int
        if (cached) return *cached;  // dereference the cached value
        auto denotation = caches.data.insert_unique(evaluate_impl(state, caches));
        caches.data.insert_mapping(key, denotation);
        return *denotation;  // dereference the newly inserted denoation
    }
    std::shared_ptr<const DenotationList> evaluate(const States& states, DenotationsCaches& caches) const {
        auto key = DenotationsCacheKey{ Base<ElementLight<Denotation, DenotationList>>::get_index(), -1, -1 };
        auto cached = caches.data.get<DenotationList>(key);
        if (cached) return cached;
        auto result_denotations = caches.data.insert_unique(evaluate_impl(states, caches));
        caches.data.insert_mapping(key, result_denotations);
        return result_denotations;
    }
};


/// @brief Represents a concept element that evaluates to a concept denotation
///        on a given state. It can also make use of a cache during evaluation.
using Concept = Element<ConceptDenotation, ConceptDenotations>;

/// @brief Represents a role element that evaluates to a role denotation
///        on a given state. It can also make use of a cache during evaluation.
using Role = Element<RoleDenotation, RoleDenotations>;

/// @brief Represents a Boolean element that evaluates to either true or false
///        on a given state. It can also make use of a cache during evaluation.
using Boolean = ElementLight<bool, BooleanDenotations>;

/// @brief Represents a numerical element that evaluates to an natural number
///        on a given state. It can also make use of a cache during evaluation.
using Numerical = ElementLight<int, NumericalDenotations>;


/// @brief Provides functionality for the syntactically unique creation of elements.
class SyntacticElementFactory {
private:
    pimpl<SyntacticElementFactoryImpl> m_pImpl;

public:
    SyntacticElementFactory(std::shared_ptr<VocabularyInfo> vocabulary_info);
    SyntacticElementFactory(const SyntacticElementFactory& other);
    SyntacticElementFactory& operator=(const SyntacticElementFactory& other);
    SyntacticElementFactory(SyntacticElementFactory&& other);
    SyntacticElementFactory& operator=(SyntacticElementFactory&& other);
    ~SyntacticElementFactory();

    std::shared_ptr<VocabularyInfo> get_vocabulary_info() const;

    /**
     * Returns a Concept if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    std::shared_ptr<const Concept> parse_concept(
        const std::string &description, const std::string& filename="");

    std::shared_ptr<const Concept> parse_concept(
        iterator_type& iter, iterator_type end, const std::string& filename="");

    /**
     * Returns a Role if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    std::shared_ptr<const Role> parse_role(
        const std::string &description, const std::string& filename="");

    std::shared_ptr<const Role> parse_role(
        iterator_type& iter, iterator_type end, const std::string& filename="");

    /**
     * Returns a Numerical if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    std::shared_ptr<const Numerical> parse_numerical(
        const std::string &description, const std::string& filename="");

    std::shared_ptr<const Numerical> parse_numerical(
        iterator_type& iter, iterator_type end, const std::string& filename="");

    /**
     * Returns a Boolean if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    std::shared_ptr<const Boolean> parse_boolean(
        const std::string &description, const std::string& filename="");

    std::shared_ptr<const Boolean> parse_boolean(
        iterator_type& iter, iterator_type end, const std::string& filename="");

    std::shared_ptr<const Boolean> make_empty_boolean(const std::shared_ptr<const Concept>& concept_);
    std::shared_ptr<const Boolean> make_empty_boolean(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Boolean> make_inclusion_boolean(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Boolean> make_inclusion_boolean(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Boolean> make_nullary_boolean(const Predicate& predicate);

    std::shared_ptr<const Concept> make_all_concept(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_);
    std::shared_ptr<const Concept> make_and_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Concept> make_bot_concept();
    std::shared_ptr<const Concept> make_diff_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Concept> make_equal_concept(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Concept> make_not_concept(const std::shared_ptr<const Concept>& concept_);
    std::shared_ptr<const Concept> make_one_of_concept(const Constant& constant);
    std::shared_ptr<const Concept> make_or_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Concept> make_projection_concept(const std::shared_ptr<const Role>& role, int pos);
    std::shared_ptr<const Concept> make_primitive_concept(const Predicate& predicate, int pos);
    std::shared_ptr<const Concept> make_some_concept(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_);
    std::shared_ptr<const Concept> make_subset_concept(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Concept> make_top_concept();

    std::shared_ptr<const Numerical> make_concept_distance_numerical(const std::shared_ptr<const Concept>& concept_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_to);
    std::shared_ptr<const Numerical> make_count_numerical(const std::shared_ptr<const Concept>& concept_);
    std::shared_ptr<const Numerical> make_count_numerical(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Numerical> make_role_distance_numerical(const std::shared_ptr<const Role>& role_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Role>& role_to);
    std::shared_ptr<const Numerical> make_sum_concept_distance_numerical(const std::shared_ptr<const Concept>& concept_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_to);
    std::shared_ptr<const Numerical> make_sum_role_distance_numerical(const std::shared_ptr<const Role>& role_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Role>& role_to);

    std::shared_ptr<const Role> make_and_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_compose_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_diff_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_identity_role(const std::shared_ptr<const Concept>& concept_);
    std::shared_ptr<const Role> make_inverse_role(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Role> make_not_role(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Role> make_or_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_primitive_role(const Predicate& predicate, int pos_1, int pos_2);
    std::shared_ptr<const Role> make_restrict_role(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_);
    std::shared_ptr<const Role> make_til_c_role(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_);
    std::shared_ptr<const Role> make_top_role();
    std::shared_ptr<const Role> make_transitive_closure(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Role> make_transitive_reflexive_closure(const std::shared_ptr<const Role>& role);
};

}

#endif
