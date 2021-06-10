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
class ConceptImpl;
class RoleImpl;
class NumericalImpl;
class BooleanImpl;
class SyntacticElementFactory;
class InstanceInfo;
class VocabularyInfo;

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

    Predicate(const VocabularyInfo& vocabulary_info, const std::string& predicate_name, int predicate_idx, int arity);
    friend class VocabularyInfoImpl;

public:
    Predicate(const Predicate& other);
    ~Predicate();

    /**
     * Getters.
     */
    const VocabularyInfo* get_vocabulary_info() const;
    int get_index() const;
    const std::string& get_name() const;
    int get_arity() const;
};


/**
 * Object contains information regarding an object in an instance.
 */
class Object {
private:
    pimpl<ObjectImpl> m_pImpl;

    Object(const InstanceInfo& instance_info, const std::string& object_name, int object_idx);
    friend class InstanceInfoImpl;

public:
    Object(const Object& other);
    ~Object();

    const InstanceInfo* get_instance_info() const;
    int get_index() const;
    const std::string& get_name() const;
};


/**
 * Atom contains information regarding mappings between name and indices.
 */
class Atom {
private:
    pimpl<AtomImpl> m_pImpl;

    Atom(const InstanceInfo& instance_info,
        const std::string& atom_name,
        int atom_idx,
        const Predicate& predicate,
        const std::vector<Object> &objects,
        bool is_static);
    friend class InstanceInfoImpl;

public:
    Atom(const Atom& other);
    ~Atom();

    /**
     * Getters.
     */
    const InstanceInfo* get_instance_info() const;
    const std::string& get_name() const;
    int get_index() const;
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
    State(std::shared_ptr<const InstanceInfo> instance_info, const std::vector<Atom>& atoms);
    State(std::shared_ptr<const InstanceInfo> instance_info, const Name_Vec& atom_names);
    State(std::shared_ptr<const InstanceInfo> instance_info, const Index_Vec& atom_indices);
    State(const State& other);
    ~State();

    /**
     * Computes string-like representation of the state.
     */
    std::string str() const;
    /**
     * Getters.
     */
    const InstanceInfo* get_instance_info() const;
    const Index_Vec& get_atom_idxs() const;
};


/**
 * VocabularyInfoImpl stores information related to the predicates in the planning domain.
 */
class VocabularyInfo {
private:
    pimpl<VocabularyInfoImpl> m_pImpl;

public:
    VocabularyInfo();
    ~VocabularyInfo();

    const Predicate& add_predicate(const std::string &predicate_name, int arity);

    bool exists_predicate_name(const std::string& name) const;
    const std::vector<Predicate>& get_predicates() const;
    int get_predicate_idx(const std::string& name) const;
    const Predicate& get_predicate(int predicate_idx) const;
};


/**
 * InstanceInfo stores information related to the planning instance.
 */
class InstanceInfo {
private:
    pimpl<InstanceInfoImpl> m_pImpl;

public:
    InstanceInfo(std::shared_ptr<const VocabularyInfo> vocabulary_info);
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
     * Getters.
     */
    const std::vector<Atom>& get_atoms() const;
    const Atom& get_atom(int atom_idx) const;
    int get_atom_idx(const std::string& name) const;
    const std::vector<Object>& get_objects() const;
    const Object& get_object(int object_idx) const;
    int get_object_idx(const std::string& object_name) const;
    int get_num_objects() const;
    const VocabularyInfo* get_vocabulary_info() const;
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
    virtual T evaluate(const State& state) = 0;

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

    Concept(const VocabularyInfo& vocabulary_info, element::Concept_Ptr&& concept);
    friend class SyntacticElementFactoryImpl;

public:
    Concept(const Concept& other);
    virtual ~Concept();

    virtual ConceptDenotation evaluate(const State& state) override;

    virtual int compute_complexity() const override;

    virtual std::string compute_repr() const override;
};


/**
 * Concept evaluates to RoleDenotation.
 */
class Role : public Element<RoleDenotation> {
protected:
    pimpl<RoleImpl> m_pImpl;

    Role(const VocabularyInfo& vocabulary_info, element::Role_Ptr&& role);
    friend class SyntacticElementFactoryImpl;

public:
    Role(const Role& other);
    virtual ~Role();

    virtual RoleDenotation evaluate(const State& state) override;

    virtual int compute_complexity() const override;

    virtual std::string compute_repr() const override;
};


/**
 * Numerical evaluates to int.
 */
class Numerical : public Element<int> {
protected:
    pimpl<NumericalImpl> m_pImpl;

    Numerical(const VocabularyInfo& vocabulary_info, element::Numerical_Ptr&& numerical);
    friend class SyntacticElementFactoryImpl;

public:
    Numerical(const Numerical& other);
    virtual ~Numerical();

    virtual int evaluate(const State& state) override;

    virtual int compute_complexity() const override;

    virtual std::string compute_repr() const override;
};


/**
 * Boolean evaluates to bool.
 */
class Boolean : public Element<bool> {
protected:
    pimpl<BooleanImpl> m_pImpl;

    Boolean(const VocabularyInfo& vocabulary_info, element::Boolean_Ptr&& boolean);
    friend class SyntacticElementFactoryImpl;

public:
    Boolean(const Boolean& other);
    virtual ~Boolean();

    virtual bool evaluate(const State& state) override;

    virtual int compute_complexity() const override;

    virtual std::string compute_repr() const override;
};


/**
 * The ElementFactory for creation and storage of elements while taking care of maintaining uniqueness.
 */
class SyntacticElementFactory {
private:
    pimpl<SyntacticElementFactoryImpl> m_pImpl;

public:
    SyntacticElementFactory(std::shared_ptr<const VocabularyInfo> vocabulary_info);
    SyntacticElementFactory(const SyntacticElementFactory& other);
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
    Concept make_primitive_concept(const std::string& name, int pos);

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
    Role make_primitive_role(const std::string& name, int pos_1, int pos_2);

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
