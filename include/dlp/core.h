#ifndef DLP_INCLUDE_DLP_CORE_H_
#define DLP_INCLUDE_DLP_CORE_H_

#include <memory>
#include <string>
#include <vector>

#include "../../src/core/elements/types.h"
#include "../../src/utils/pimpl.h"


namespace dlp {
namespace core {
class SyntacticElementFactoryImpl;
class InstanceInfoImpl;
class PredicateImpl;
class AtomImpl;
class StateImpl;
class SyntacticElementFactory;

using ConceptDenotation = std::vector<int>;

using RoleDenotation = std::vector<std::pair<int, int>>;

using Name_Vec = std::vector<std::string>;
using Index_Vec = std::vector<int>;


/**
 * Predicate contains information regarding the predicates used to construct the atoms.
 */
class Predicate {
private:
    pimpl<PredicateImpl> m_pImpl;
    std::shared_ptr<InstanceInfoImpl> m_parent;

    Predicate(std::shared_ptr<InstanceInfoImpl> parent, PredicateImpl&& impl);

    friend class InstanceInfo;

public:
    Predicate() = delete;
    Predicate(const Predicate& other);
    ~Predicate();

    /**
     * Getters.
     */
    const std::string& get_name() const;
    unsigned get_arity() const;
};


/**
 * Atom contains information regarding mappings between name and indices.
 */
class Atom {
private:
    pimpl<AtomImpl> m_pImpl;
    std::shared_ptr<InstanceInfoImpl> m_parent;

    Atom(std::shared_ptr<InstanceInfoImpl> parent, AtomImpl&& impl);

    friend class InstanceInfo;

public:
    Atom() = delete;
    Atom(const Atom& other);
    ~Atom();

    /**
     * Getters.
     */
    int get_atom_idx() const;
    const std::string& get_atom_name() const;
};


/**
 * State contains static and dynamic atoms.
 */
class State {
private:
    pimpl<StateImpl> m_pImpl;
    std::shared_ptr<InstanceInfoImpl> m_parent;

    State(std::shared_ptr<InstanceInfoImpl> parent, StateImpl&& impl);

    friend class InstanceInfo;
    friend class Concept;
    friend class Role;
    friend class Numerical;
    friend class Boolean;

public:
    State() = delete;
    State(const State& other);
    ~State();

    /**
     * Computes string-like representation of the state.
     */
    std::string str() const;
};


/**
 * InstanceInfo stores information related to the planning instance.
 */
class InstanceInfo {
private:
    pimpl<std::shared_ptr<InstanceInfoImpl>> m_pImpl;

    friend class SyntacticElementFactory;

public:
    InstanceInfo();
    InstanceInfo(const InstanceInfo& other) = delete;
    ~InstanceInfo();

    /**
     * Adds an atom that may have varying evaluation depending on the state.
     */
    Atom add_atom(const std::string& predicate_name, const Name_Vec& object_names);

    /**
     * Adds an atom that remains true forever.
     */
    Atom add_static_atom(const std::string& predicate_name, const Name_Vec& object_names);

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
     * Getters.
     */
    std::vector<Predicate> get_predicates() const;
};


/**
 * Abstract base class of any Element.
 */
template<typename T>
class Element {
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
    virtual unsigned compute_complexity() const = 0;

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
    pimpl<element::Concept_Ptr> m_pImpl;

    Concept(element::Concept_Ptr pImpl);

    friend class SyntacticElementFactory;

public:
    Concept() = delete;
    Concept(const Concept& other);
    virtual ~Concept() = default;

    virtual ConceptDenotation evaluate(const State& state) const override;

    virtual unsigned compute_complexity() const override;

    virtual std::string compute_repr() const override;
};


/**
 * Concept evaluates to RoleDenotation.
 */
class Role : public Element<RoleDenotation> {
protected:
    pimpl<element::Role_Ptr> m_pImpl;

    Role(element::Role_Ptr pImpl);

    friend class SyntacticElementFactory;

public:
    Role() = delete;
    Role(const Role& other);
    virtual ~Role() = default;

    virtual RoleDenotation evaluate(const State& state) const override;

    virtual unsigned compute_complexity() const override;

    virtual std::string compute_repr() const override;
};


/**
 * Numerical evaluates to int.
 */
class Numerical : public Element<int> {
protected:
    pimpl<element::Numerical_Ptr> m_pImpl;

    Numerical(element::Numerical_Ptr pImpl);

    friend class SyntacticElementFactory;

public:
    Numerical() = delete;
    Numerical(const Numerical& other);
    virtual ~Numerical() = default;

    virtual int evaluate(const State& state) const override;

    virtual unsigned compute_complexity() const override;

    virtual std::string compute_repr() const override;
};


/**
 * Boolean evaluates to bool.
 */
class Boolean : public Element<bool> {
protected:
    pimpl<element::Boolean_Ptr> m_pImpl;

    Boolean(element::Boolean_Ptr pImpl);

    friend class SyntacticElementFactory;

public:
    Boolean() = delete;
    Boolean(const Boolean& other);
    virtual ~Boolean() = default;

    virtual bool evaluate(const State& state) const override;

    virtual unsigned compute_complexity() const override;

    virtual std::string compute_repr() const override;
};


/**
 * The ElementFactory for creation and storage of elements while taking care of maintaining uniqueness.
 */
class SyntacticElementFactory {
private:
    pimpl<SyntacticElementFactoryImpl> m_pImpl;

public:
    SyntacticElementFactory();
    SyntacticElementFactory(const SyntacticElementFactory& other) = delete;
    ~SyntacticElementFactory();

    /**
     * Returns a Concept if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    Concept parse_concept_element(const std::string &description);

    /**
     * Returns a Role if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    Role parse_role_element(const std::string &description);

    /**
     * Returns a Numerical if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    Numerical parse_numerical_element(const std::string &description);

    /**
     * Returns a Boolean if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    Boolean parse_boolean_element(const std::string &description);

    /**
     * Returns a Boolean that evaluates to true iff the Concept evaluates to the empty set.
     */
    Boolean make_empty_boolean_element(const Concept& concept);

    /**
     * Returns a Boolean that evaluates to true iff the Role evaluates to the empty set.
     */
    Boolean make_empty_boolean_element(const Role& role);

    /**
     * Returns a Concept that evaluates to the universal abstraction.
     */
    Concept make_all_concept_element(const Role& role, const Concept& concept);

    /**
     * Returns a Concept that evaluates to the intersection of the operand evaluations.
     */
    Concept make_and_concept_element(const Concept& concept_left, const Concept& concept_right);

    /**
     * Returns a Concept that evaluates to the empty set.
     */
    Concept make_bot_concept_element();

    /**
     * Returns a Concept that evaluates to the set difference.
     */
    Concept make_diff_concept_element(const Concept& concept_left, const Concept& concept_right);

    /**
     * Returns a ConceptElements that evaluates to all elements that are not contained in the evaluation result of the given Concept.
     */
    Concept make_not_concept_element(const Concept& concept);

    /**
     * Returns a Concept that evaluates to the single object with the given index.
     */
    Concept make_one_of_concept_element(unsigned object_idx);

    /**
     * Returns a Concept that evaluates to the union of the operand evaluations.
     */
    Concept make_or_concept_element(const Concept& concept_left, const Concept& concept_right);

    /**
     * Returns a Concept that evaluates to the concept obtained by projecting atoms in a given state to the column at pos.
     */
    Concept make_primitive_concept_element(const std::string& name, unsigned pos);

    /**
     * Returns a Concept that evaluates the existential abstraction.
     */
    Concept make_some_concept_element(const Role& role, const Concept& concept);

    /**
     * Returns a Concept that evaluates the role value mapping.
     */
    Concept make_subset_concept_element(const Role& role_left, const Role& role_right);

    /**
     * Returns a Concept that evaluates to the universe, i.e., the set of all objects.
     */
    Concept make_top_concept_element();

    /**
     *
     */
    Numerical make_concept_distance_element(const Concept& concept_from, const Role& role, const Concept& concept_to);

    /**
     * Returns a Numerical that counts the number of concept.
     */
    Numerical make_count_element(const Concept& concept);

    /**
     * Returns a Numerical that counts the number of roles.
     */
    Numerical make_count_element(const Role& role);

    /**
     *
     */
    Numerical make_role_distance_element(const Role& role_from, const Role& role, const Role& role_to);

    /**
     *
     */
    Numerical make_sum_concept_distance_element(const Concept& concept_from, const Role& role, const Concept& concept_to);

    /**
     *
     */
    Numerical make_sum_role_distance_element(const Role& role_from, const Role& role, const Role& role_to);

    /**
     * Returns a Role that evaluates to the intersection of the operand evaluations.
     */
    Role make_and_role_element(const Role& role_left, const Role& role_right);

    /**
     * Returns a Role that evaluates to the composition of the operand evaluations.
     */
    Role make_compose_role_element(const Role& role_left, const Role& role_right);

    /**
     * Returns a Role that evaluates to the set difference of the operand evaluations.
     */
    Role make_diff_role_element(const Role& role_left, const Role& role_right);

    /**
     * Returns a Role that evaluates to the identity mapping of the concept in the evaluation result.
     */
    Role make_identity_role_element(const Concept& concept);

    /**
     * Returns a Role that evaluates to the inverse roles of the roles in the evaluation result.
     */
    Role make_inverse_role_element(const Role& role);

    /**
     * Returns a RoleElements that evaluates to all roles that are not contined in the evaluation result.
     */
    Role make_not_role_element(const Role& role);

    /**
     * Returns a Role that evaluates to the set union of the operand evaluations.
     */
    Role make_or_role_element(const Role& role_left, const Role& role_right);

    /**
     * Returns a Role that evaluates to the roles obtained by projecting atoms in a given state to the columns at pos_1 and pos_2.
     */
    Role make_primitive_role_element(const std::string& name, unsigned pos_1, unsigned pos_2);

    /**
     * Returns a Role that evaluates to the role restriction.
     */
    Role make_restrict_role_element(const Role& role, const Concept& concept);

    /**
     * Returns a Role that evaluates to the set of all roles.
     */
    Role make_top_role_element();

    /**
     * Returns a Role that evaluates to the transitive closure over the role.
     */
    Role make_transitive_closure_element(const Role& role);

    /**
     * Returns a Role that evaluates to the transitive reflexive closure over the role.
     */
    Role make_transitive_reflexive_closure_element(const Role& role);
};


}
}

#endif
