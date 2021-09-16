#ifndef DLPLAN_INCLUDE_DLP_CORE_H_
#define DLPLAN_INCLUDE_DLP_CORE_H_

#include <memory>
#include <string>
#include <vector>

#include "pimpl.h"


namespace dlplan::core {
class SyntacticElementFactoryImpl;
class InstanceInfoImpl;
class VocabularyInfoImpl;
class PredicateImpl;
class ConstantImpl;
class ObjectImpl;
class AtomImpl;
class StateImpl;
class ConceptImpl;
class RoleImpl;
class NumericalImpl;
class BooleanImpl;
class SyntacticElementFactory;
class InstanceInfo;
class VocabularyInfo;
namespace element {
    class Concept;
    class Role;
    class Numerical;
    class Boolean;
}

using ConceptDenotation = std::vector<int>;

using RoleDenotation = std::vector<std::pair<int, int>>;

using Name_Vec = std::vector<std::string>;
using Index_Vec = std::vector<int>;

class Constant {
private:
    pimpl<ConstantImpl> m_pImpl;

    Constant(const std::string& name, int index);
    friend class VocabularyInfoImpl;

public:
    Constant(const Constant& other);
    Constant& operator=(const Constant& other);
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
    pimpl<PredicateImpl> m_pImpl;

    Predicate(const std::string& name, int index, int arity);
    friend class VocabularyInfoImpl;

public:
    Predicate(const Predicate& other);
    Predicate& operator=(const Predicate& other);
    ~Predicate();

    bool operator==(const Predicate& other) const;
    bool operator!=(const Predicate& other) const;

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
    pimpl<ObjectImpl> m_pImpl;

    Object(const std::string& name, int index);
    friend class InstanceInfoImpl;

public:
    Object(const Object& other);
    Object& operator=(const Object& other);
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
    pimpl<AtomImpl> m_pImpl;

    Atom(const std::string& name,
        int index,
        const Predicate& predicate,
        const std::vector<Object> &objects,
        bool is_static);
    friend class InstanceInfoImpl;

public:
    Atom(const Atom& other);
    Atom& operator=(const Atom& other);
    ~Atom();

    bool operator==(const Atom& other) const;
    bool operator!=(const Atom& other) const;

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
    pimpl<StateImpl> m_pImpl;

public:
    State(std::shared_ptr<const InstanceInfo> instance_info, const std::vector<Atom>& atoms);
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
};


/**
 * InstanceInfo stores information related to the planning instance.
 */
class InstanceInfo {
private:
    pimpl<InstanceInfoImpl> m_pImpl;

public:
    InstanceInfo(std::shared_ptr<const VocabularyInfo> vocabulary_info);
    InstanceInfo& operator=(const InstanceInfo& other);
    InstanceInfo() = delete;
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
};


/**
 * Abstract base class of any Element.
 */
template<typename T>
class Element {
protected:
    Element() = default;

public:
    virtual ~Element() = default;

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
};


/**
 * Concept evaluates to ConceptDenotation.
 */
class Concept : public Element<ConceptDenotation> {
protected:
    pimpl<ConceptImpl> m_pImpl;

    Concept(const VocabularyInfo& vocabulary_info, std::shared_ptr<element::Concept>&& concept);
    friend class SyntacticElementFactoryImpl;

public:
    Concept(const Concept& other);
    Concept& operator=(const Concept& other);
    ~Concept() override;

    ConceptDenotation evaluate(const State& state) const override;

    int compute_complexity() const override;

    std::string compute_repr() const override;
};


/**
 * Concept evaluates to RoleDenotation.
 */
class Role : public Element<RoleDenotation> {
protected:
    pimpl<RoleImpl> m_pImpl;

    Role(const VocabularyInfo& vocabulary_info, std::shared_ptr<element::Role>&& role);
    friend class SyntacticElementFactoryImpl;

public:
    Role(const Role& other);
    Role& operator=(const Role& other);
    ~Role() override;

    RoleDenotation evaluate(const State& state) const override;

    int compute_complexity() const override;

    std::string compute_repr() const override;
};


/**
 * Numerical evaluates to int.
 */
class Numerical : public Element<int> {
protected:
    pimpl<NumericalImpl> m_pImpl;

    Numerical(const VocabularyInfo& vocabulary_info, std::shared_ptr<element::Numerical>&& numerical);
    friend class SyntacticElementFactoryImpl;

public:
    Numerical(const Numerical& other);
    Numerical& operator=(const Numerical& other);
    ~Numerical() override;

    int evaluate(const State& state) const override;

    int compute_complexity() const override;

    std::string compute_repr() const override;
};


/**
 * Boolean evaluates to bool.
 */
class Boolean : public Element<bool> {
protected:
    pimpl<BooleanImpl> m_pImpl;

    Boolean(const VocabularyInfo& vocabulary_info, std::shared_ptr<element::Boolean>&& boolean);
    friend class SyntacticElementFactoryImpl;

public:
    Boolean(const Boolean& other);
    Boolean& operator=(const Boolean& other);
    ~Boolean() override;

    bool evaluate(const State& state) const override;

    int compute_complexity() const override;

    std::string compute_repr() const override;
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

#endif
