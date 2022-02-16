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
#include "per_index_array.h"
#include "per_index_bitset.h"
#include "phmap.h"


namespace dlplan::core {
class SyntacticElementFactoryImpl;
class InstanceInfoImpl;
class VocabularyInfoImpl;
class SyntacticElementFactory;
class InstanceInfo;
class VocabularyInfo;
class Boolean;
class Numerical;
class Concept;
class Role;
class State;
struct PerElementEvaluationContext;
struct PerElementAndStateEvaluationContext;
namespace element {
    template<typename T>
    class Element;
    class Concept;
    class Role;
    class Numerical;
    class Boolean;
}

/**
 * Proxy to underlying Bitset with additional functionality.
 */
class ConceptDenotation {
private:
    // no pimpl to save indirection.
    int m_num_objects;
    utils::BitsetView m_data;

public:
    ConceptDenotation(int num_objects, utils::BitsetView data);
    ~ConceptDenotation();

    /**
     * TODO: must ensure that elements are in some canonical representation.
     * This function is not intended to be called by the user due to the
     * additional overhead but it is useful for testing.
     */
    std::vector<int> to_vector() const;

    int get_num_objects() const;
    utils::BitsetView& get_data();
    const utils::BitsetView& get_data() const;
};

/**
 * Proxy to underlying Bitset with additional functionality.
 */
class RoleDenotation {
private:
    // no pimpl to save indirection.
    int m_num_objects;
    utils::BitsetView m_data;

public:
    RoleDenotation(int num_objects, utils::BitsetView data);
    ~RoleDenotation();

    std::vector<std::pair<int, int>> to_vector() const;

    int get_num_objects() const;
    utils::BitsetView& get_data();
    const utils::BitsetView& get_data() const;
};

/**
 * Provides a cache entry for the result of evaluating an element.
 *
 * This cache is useful when evaluating a set of elements
 * that use the same sub-elements on the same state.
 */
class PerElementEvaluationCache {
private:
    /**
     * Fragmented indexing scheme to indexing scheme
     * and existing entries indicate that the result is cached.
     * Element indices can be fragmented because we allow deletion.
     */
    phmap::flat_hash_map<int, int> m_concept_index_to_cache_index;
    phmap::flat_hash_map<int, int> m_role_index_to_cache_index;

    int m_num_objects;

    utils::PerIndexBitset m_concept_denot_cache;
    utils::PerIndexBitset m_role_denot_cache;
    // Booleans and numericals are not interesting for caching
    // because they can be computed quickly from cached concept
    // and role denotations.

    std::shared_ptr<State> m_cached_state;

public:
    PerElementEvaluationCache(int num_objects);
    PerElementEvaluationCache(const PerElementEvaluationCache& other) = delete;
    PerElementEvaluationCache& operator=(const PerElementEvaluationCache& other) = delete;
    ~PerElementEvaluationCache();

    /**
     * Passing a different state will automatically clear the cache.
     * Ownership is passed because we need to check whether
     * the state has changed in comparison to previous evaluation.
     */
    ConceptDenotation retrieve_or_evaluate(const element::Concept& concept, PerElementEvaluationContext& context);
    RoleDenotation retrieve_or_evaluate(const element::Role& role, PerElementEvaluationContext& context);

    /**
     * We have to clear the cache if we identify in the root element
     * that the state has changed in comparison to the previous evaluation.
     */
    void clear_if_state_changed(std::shared_ptr<State> state);
};

struct PerElementEvaluationContext {
    PerElementEvaluationCache& cache;
    std::shared_ptr<State> state;

    PerElementEvaluationContext(PerElementEvaluationCache& cache, std::shared_ptr<State> state)
        : cache(cache), state(state) {
        // TODO: error checking cache and state must have same instance_info
    }
};

/**
 * Provides a cache entry for the result of evaluating an element on a given state.
 *
 * This cache is useful if one wants to evaluate Boolean and Numericals
 * with the same state multiple times but PerElementEvaluationCache
 * fails to cache the result because different states are evaluated in between.
 *
 * An example usage is a Policy where a target state
 * becomes the new source state if a transition is classified as good.
 */
//class PerElementAndStateEvaluationCache {
//private:
//    utils::PerIndexBitset m_boolean_denots_cache;
//    utils::PerIndexArray<int> m_numerical_denots_cache;
//    // Concepts and roles are not interesting for caching because
//    // we usually do not refer to them during search.
//
//public:
//    PerElementAndStateEvaluationCache(int num_booleans, int num_numericals) { }
//
//    bool retrieve_or_evaluate(const element::Boolean& boolean, PerElementAndStateEvaluationContext& context);
//    int retrieve_or_evaluate(const element::Numerical& numerical, PerElementAndStateEvaluationContext& context);
//
//    /**
//     * User can manually clear the cache or free memory.
//     * This functionality is useful when running
//     * a sequence of search algorithms,
//     * where each search generates different sets of states.
//     * A concrete example is Serialized Iterated Width.
//     */
//    void clear();
//    void free();
//};
//
//struct PerElementAndStateEvaluationContext {
//    PerElementEvaluationCache& element_cache;
//    PerElementAndStateEvaluationCache& element_and_state_cache;
//    // TODO: think of saving the index directly in the state.
//    int state_idx;
//    std::shared_ptr<State> state;
//};


class Constant {
private:
    std::string m_name;
    int m_index;

    Constant(const std::string& name, int index);
    friend class VocabularyInfoImpl;

public:
    Constant() = delete;
    Constant(const Constant& other);
    Constant& operator=(const Constant& other);
    ~Constant();

    bool operator==(const Constant& other) const;
    bool operator!=(const Constant& other) const;

    std::string str() const;

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

    Predicate(const std::string& name, int index, int arity);
    friend class VocabularyInfoImpl;

public:
    Predicate() = delete;
    Predicate(const Predicate& other);
    Predicate& operator=(const Predicate& other);
    ~Predicate();

    bool operator==(const Predicate& other) const;
    bool operator!=(const Predicate& other) const;

    std::string str() const;

    /**
     * Getters.
     */
    int get_index() const;
    const std::string& get_name() const;
    int get_arity() const;
};


/**
 * An Object belongs to a specific instance.
 */
class Object {
private:
    std::string m_name;
    int m_index;

    Object(const std::string& name, int index);
    friend class InstanceInfoImpl;

public:
    Object() = delete;
    Object(const Object& other);
    Object& operator=(const Object& other);
    ~Object();

    bool operator==(const Object& other) const;
    bool operator!=(const Object& other) const;

    std::string str() const;

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
    Predicate m_predicate;
    std::vector<Object> m_objects;
    bool m_is_static;

    Atom(const std::string& name,
        int index,
        const Predicate& predicate,
        const std::vector<Object> &objects,
        bool is_static);
    friend class InstanceInfoImpl;

public:
    Atom() = delete;
    Atom(const Atom& other);
    Atom& operator=(const Atom& other);
    ~Atom();

    bool operator==(const Atom& other) const;
    bool operator!=(const Atom& other) const;

    std::string str() const;

    /**
     * Getters.
     */
    const std::string& get_name() const;
    int get_index() const;
    const Predicate& get_predicate() const;
    const std::vector<Object>& get_objects() const;
    const Object& get_object(int pos) const;
    bool get_is_static() const;
};


/**
 * A State contains consists of atoms including all static atoms.
 */
class State {
private:
    std::shared_ptr<const InstanceInfo> m_instance_info;
    Index_Vec m_atom_idxs;

    phmap::flat_hash_map<int, std::vector<int>> m_per_predicate_idx_static_atom_idxs;

public:
    State() = delete;
    State(std::shared_ptr<const InstanceInfo> instance_info, const std::vector<Atom>& atoms);
    State(std::shared_ptr<const InstanceInfo> instance_info, const Index_Vec& atom_idxs);
    State(const State& other);
    State& operator=(const State& other);
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
    const phmap::flat_hash_map<int, std::vector<int>>& get_per_predicate_idx_static_atom_idxs() const;

    size_t compute_hash() const;
};


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

    size_t compute_hash() const;
};


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
    ~InstanceInfo();

    /**
     * Adds an atom that may have varying evaluation depending on the state.
     */
    const Atom& add_atom(const std::string& name, const Name_Vec& object_names, bool negated=false);

    /**
     * Adds an atom that remains true forever.
     */
    const Atom& add_static_atom(const std::string& name, const Name_Vec& object_names);

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
};


/**
 * Abstract base class of any Element.
 */
template<typename T>
class Element {
protected:
    const std::shared_ptr<const VocabularyInfo> m_vocabulary_info;

protected:
    Element(std::shared_ptr<const VocabularyInfo> vocabulary_info);

public:
    virtual ~Element();

    /**
     * Evaluates the element for a state given as a vector of atom indices.
     */
    virtual T evaluate(PerElementEvaluationContext& context) const = 0;

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
    const std::shared_ptr<const element::Concept> m_element;

    Concept(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const element::Concept>&& concept);
    friend class SyntacticElementFactoryImpl;

public:
    ~Concept() override;

    ConceptDenotation evaluate(PerElementEvaluationContext& context) const override;

    int compute_complexity() const override;

    std::string compute_repr() const override;

    std::shared_ptr<const element::Concept> get_element() const;
};


/**
 * Concept evaluates to RoleDenotation.
 */
class Role : public Element<RoleDenotation> {
private:
    const std::shared_ptr<const element::Role> m_element;

    Role(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const element::Role>&& role);
    friend class SyntacticElementFactoryImpl;

public:
    ~Role() override;

    RoleDenotation evaluate(PerElementEvaluationContext& context) const override;

    int compute_complexity() const override;

    std::string compute_repr() const override;

    std::shared_ptr<const element::Role> get_element() const;
};


/**
 * Numerical evaluates to int.
 */
class Numerical : public Element<int> {
private:
    const std::shared_ptr<const element::Numerical> m_element;

    Numerical(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const element::Numerical>&& numerical);
    friend class SyntacticElementFactoryImpl;

public:
    ~Numerical() override;

    int evaluate(PerElementEvaluationContext& context) const override;

    int compute_complexity() const override;

    std::string compute_repr() const override;

    std::shared_ptr<const element::Numerical> get_element() const;
};


/**
 * Boolean evaluates to bool.
 */
class Boolean : public Element<bool> {
private:
    const std::shared_ptr<const element::Boolean> m_element;

    Boolean(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const element::Boolean>&& boolean);
    friend class SyntacticElementFactoryImpl;

public:
    ~Boolean() override;

    bool evaluate(PerElementEvaluationContext& context) const override;

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

namespace std {
    template <>
    struct hash<dlplan::core::State> {
        std::size_t operator()(const dlplan::core::State& state) const {
            return state.compute_hash();
        }
    };
}

#endif
