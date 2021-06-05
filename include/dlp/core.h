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
class VocabularyInfoImpl;
class PredicateImpl;
class ObjectImpl;
class AtomImpl;
class StateImpl;
class SyntacticElementFactory;
class InstanceInfo;

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

public:
    Predicate(PredicateImpl&& impl);
    Predicate(const Predicate& other);
    ~Predicate();

    /**
     * Getters.
     */
    const VocabularyInfoImpl* get_vocabulary_info() const;
    int get_predicate_idx() const;
    const std::string& get_name() const;
    unsigned get_arity() const;
};


/**
 * Object contains information regarding an object in an instance.
 */
class Object {
private:
    pimpl<ObjectImpl> m_pImpl;

public:
    Object(ObjectImpl&& impl);
    Object(const Object& other);
    ~Object();

    const InstanceInfoImpl* get_instance_info() const;
    int get_object_idx() const;
    const std::string& get_object_name() const;
};


/**
 * Atom contains information regarding mappings between name and indices.
 */
class Atom {
private:
    pimpl<AtomImpl> m_pImpl;

public:
    Atom(AtomImpl&& impl);
    Atom(const Atom& other);
    ~Atom();

    /**
     * Getters.
     */
    const InstanceInfoImpl* get_instance_info() const;
    const std::string& get_atom_name() const;
    int get_atom_idx() const;
    const Predicate& get_predicate() const;
    const std::vector<Object>& get_objects() const;
    const Object& get_object(int pos) const;
    bool get_is_static() const;
};


/**
 * State contains static and dynamic atoms.
 */
class State {
private:
    pimpl<StateImpl> m_pImpl;

public:
    State(StateImpl&& impl);
    State(const State& other);
    ~State();

    /**
     * Computes string-like representation of the state.
     */
    std::string str() const;
    /**
     * Getters.
     */
    std::shared_ptr<const InstanceInfoImpl> get_instance_info() const;
    const Index_Vec& get_atom_idxs() const;
};


/**
 * VocabularyInfoImpl stores information related to the predicates in the planning domain.
 */
class VocabularyInfo {
private:
    spimpl<VocabularyInfoImpl> m_pImpl;

public:
    VocabularyInfo();
    ~VocabularyInfo();

    const Predicate& add_predicate(const std::string &predicate_name, unsigned arity);

    InstanceInfo make_instance();

    SyntacticElementFactory make_factory();
};


/**
 * InstanceInfo stores information related to the planning instance.
 */
class InstanceInfo {
private:
    spimpl<InstanceInfoImpl> m_pImpl;

public:
    InstanceInfo(InstanceInfoImpl&& impl);
    InstanceInfo() = delete;
    ~InstanceInfo();

    /**
     * Adds an atom that may have varying evaluation depending on the state.
     */
    const Atom& add_atom(const std::string& predicate_name, const Name_Vec& object_names);

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
     * Getters.
     */
    const std::shared_ptr<const VocabularyInfoImpl>& get_vocabulary_info() const;
};


/**
 * Abstract base class of any Element.
 */
template<typename T>
class Element {
protected:
    const VocabularyInfoImpl* m_parent;  // non-owning

public:
    Element(const VocabularyInfoImpl& parent) : m_parent(&parent) { }
    Element(const Element& other) : m_parent(other.m_parent) { }
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

    Concept(const VocabularyInfoImpl& parent, element::Concept_Ptr pImpl);

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

    Role(const VocabularyInfoImpl& parent, element::Role_Ptr pImpl);

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

    Numerical(const VocabularyInfoImpl& parent, element::Numerical_Ptr pImpl);

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

    Boolean(const VocabularyInfoImpl& parent, element::Boolean_Ptr pImpl);

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
    SyntacticElementFactory(SyntacticElementFactoryImpl&& impl);
    SyntacticElementFactory(const SyntacticElementFactory& other);
    ~SyntacticElementFactory();

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

    /**
     * Returns a Boolean that evaluates to true iff the Concept evaluates to the empty set.
     */
    Boolean make_empty_boolean(const Concept& concept);

    /**
     * Returns a Boolean that evaluates to true iff the Role evaluates to the empty set.
     */
    Boolean make_empty_boolean(const Role& role);

    /**
     * Returns a Concept that evaluates to the universal abstraction.
     */
    Concept make_all_concept(const Role& role, const Concept& concept);

    /**
     * Returns a Concept that evaluates to the intersection of the operand evaluations.
     */
    Concept make_and_concept(const Concept& concept_left, const Concept& concept_right);

    /**
     * Returns a Concept that evaluates to the empty set.
     */
    Concept make_bot_concept();

    /**
     * Returns a Concept that evaluates to the set difference.
     */
    Concept make_diff_concept(const Concept& concept_left, const Concept& concept_right);

    /**
     * Returns a ConceptElements that evaluates to all elements that are not contained in the evaluation result of the given Concept.
     */
    Concept make_not_concept(const Concept& concept);

    /**
     * Returns a Concept that evaluates to the single object with the given index.
     */
    Concept make_one_of_concept(const std::string& object_name);

    /**
     * Returns a Concept that evaluates to the union of the operand evaluations.
     */
    Concept make_or_concept(const Concept& concept_left, const Concept& concept_right);

    /**
     * Returns a Concept that evaluates to the concept obtained by projecting atoms in a given state to the column at pos.
     */
    Concept make_primitive_concept(const std::string& name, unsigned pos);

    /**
     * Returns a Concept that evaluates the existential abstraction.
     */
    Concept make_some_concept(const Role& role, const Concept& concept);

    /**
     * Returns a Concept that evaluates the role value mapping.
     */
    Concept make_subset_concept(const Role& role_left, const Role& role_right);

    /**
     * Returns a Concept that evaluates to the universe, i.e., the set of all objects.
     */
    Concept make_top_concept();

    /**
     *
     */
    Numerical make_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to);

    /**
     * Returns a Numerical that counts the number of concept.
     */
    Numerical make_count(const Concept& concept);

    /**
     * Returns a Numerical that counts the number of roles.
     */
    Numerical make_count(const Role& role);

    /**
     *
     */
    Numerical make_role_distance(const Role& role_from, const Role& role, const Role& role_to);

    /**
     *
     */
    Numerical make_sum_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to);

    /**
     *
     */
    Numerical make_sum_role_distance(const Role& role_from, const Role& role, const Role& role_to);

    /**
     * Returns a Role that evaluates to the intersection of the operand evaluations.
     */
    Role make_and_role(const Role& role_left, const Role& role_right);

    /**
     * Returns a Role that evaluates to the composition of the operand evaluations.
     */
    Role make_compose_role(const Role& role_left, const Role& role_right);

    /**
     * Returns a Role that evaluates to the set difference of the operand evaluations.
     */
    Role make_diff_role(const Role& role_left, const Role& role_right);

    /**
     * Returns a Role that evaluates to the identity mapping of the concept in the evaluation result.
     */
    Role make_identity_role(const Concept& concept);

    /**
     * Returns a Role that evaluates to the inverse roles of the roles in the evaluation result.
     */
    Role make_inverse_role(const Role& role);

    /**
     * Returns a RoleElements that evaluates to all roles that are not contined in the evaluation result.
     */
    Role make_not_role(const Role& role);

    /**
     * Returns a Role that evaluates to the set union of the operand evaluations.
     */
    Role make_or_role(const Role& role_left, const Role& role_right);

    /**
     * Returns a Role that evaluates to the roles obtained by projecting atoms in a given state to the columns at pos_1 and pos_2.
     */
    Role make_primitive_role(const std::string& name, unsigned pos_1, unsigned pos_2);

    /**
     * Returns a Role that evaluates to the role restriction.
     */
    Role make_restrict_role(const Role& role, const Concept& concept);

    /**
     * Returns a Role that evaluates to the set of all roles.
     */
    Role make_top_role();

    /**
     * Returns a Role that evaluates to the transitive closure over the role.
     */
    Role make_transitive_closure(const Role& role);

    /**
     * Returns a Role that evaluates to the transitive reflexive closure over the role.
     */
    Role make_transitive_reflexive_closure(const Role& role);
};


}
}

#endif
