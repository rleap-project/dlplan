#ifndef DLP_INCLUDE_DLP_CORE_H_
#define DLP_INCLUDE_DLP_CORE_H_

#include <memory>
#include <string>
#include <vector>

#include "../../src/core/elements/types.h"
#include "../../src/utils/pimpl.h"


namespace dlp {
namespace core {
class ElementFactoryImpl;
class InstanceInfoImpl;
class AtomImpl;
class StateImpl;
class ElementFactory;

using Concept = int;
using Concepts = std::vector<Concept>;

using Role = std::pair<Concept, Concept>;
using Roles = std::vector<Role>;

using Name_Vec = std::vector<std::string>;
using Index_Vec = std::vector<int>;


/**
 * Atom contains information regarding mappings between name and indices.
 */
class Atom {
private:
    pimpl<AtomImpl> m_pImpl;

    Atom(const AtomImpl& impl);

    friend class InstanceInfo;

public:
    Atom() = delete;
    Atom(const Atom& other);
    ~Atom();

    /**
     * Getters.
     */
    int atom_idx() const;
};

/**
 * State contains static and dynamic atoms.
 */
class State {
private:
    pimpl<StateImpl> m_pImpl;

    State(StateImpl impl);

    friend class InstanceInfo;
    friend class ConceptElement;
    friend class RoleElement;
    friend class NumericalElement;
    friend class BooleanElement;

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

    friend class ElementFactory;

public:
    InstanceInfo();
    ~InstanceInfo();

    /**
     * Adds an atom and internally extends a mapping from names to indices.
     */
    Atom add_atom(const std::string& predicate_name, const Name_Vec& object_names, bool is_static);

    /**
     * Construct a state from textual information by first applying the index mapping and the calling convert_state.
     */
    State parse_state(/* tba */) const;
    /**
     * Constructs a state from atom indices by extending with the static and goal atoms of the instance.
     */
    State convert_state(const Index_Vec& atom_idxs);
};

/**
 * Abstract base class of any Element.
 */
template<typename T>
class Element {
public:
    Element() = default;
    virtual ~Element() = default;

    /**
     * Evaluates the element for a state given as a vector of atom indices.
     */
    virtual T evaluate(const State& state) const = 0;

    /**
     * Returns the complexity of the element
     * measured in the size of the abstract syntax tree.
     */
    virtual unsigned complexity() const = 0;
};


/**
 * ConceptElement evaluates to Concepts.
 */
class ConceptElement : public Element<Concepts> {
protected:
    pimpl<element::ConceptElement_Ptr> m_pImpl;

    ConceptElement(element::ConceptElement_Ptr pImpl);

    friend class ElementFactory;

public:
    ConceptElement() = delete;
    ConceptElement(const ConceptElement& other);
    virtual ~ConceptElement() = default;

    virtual Concepts evaluate(const State& state) const override;

    virtual unsigned complexity() const override;
};


/**
 * ConceptElement evaluates to Roles.
 */
class RoleElement : public Element<Roles> {
protected:
    pimpl<element::RoleElement_Ptr> m_pImpl;

    RoleElement(element::RoleElement_Ptr pImpl);

    friend class ElementFactory;

public:
    RoleElement() = delete;
    RoleElement(const RoleElement& other);
    virtual ~RoleElement() = default;

    virtual Roles evaluate(const State& state) const override;

    virtual unsigned complexity() const override;
};


/**
 * NumericalElement evaluates to int.
 */
class NumericalElement : public Element<int> {
protected:
    pimpl<element::NumericalElement_Ptr> m_pImpl;

    NumericalElement(element::NumericalElement_Ptr pImpl);

    friend class ElementFactory;

public:
    NumericalElement() = delete;
    NumericalElement(const NumericalElement& other);
    virtual ~NumericalElement() = default;

    virtual int evaluate(const State& state) const override;

    virtual unsigned complexity() const override;
};


/**
 * BooleanElement evaluates to bool.
 */
class BooleanElement : public Element<bool> {
protected:
    pimpl<element::BooleanElement_Ptr> m_pImpl;

    BooleanElement(element::BooleanElement_Ptr pImpl);

    friend class ElementFactory;

public:
    BooleanElement() = delete;
    BooleanElement(const BooleanElement& other);
    virtual ~BooleanElement() = default;

    virtual bool evaluate(const State& state) const override;

    virtual unsigned complexity() const override;
};


/**
 * The ElementFactory for creation and storage of elements while taking care of maintaining uniqueness.
 */
class ElementFactory {
private:
    pimpl<ElementFactoryImpl> m_pImpl;

public:
    ElementFactory();
    ~ElementFactory();

    /**
     * Returns a ConceptElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    ConceptElement parse_concept_element(const InstanceInfo& info, const std::string &description);

    /**
     * Returns a RoleElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    RoleElement parse_role_element(const InstanceInfo& info, const std::string &description);

    /**
     * Returns a NumericalElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    NumericalElement parse_numerical_element(const InstanceInfo& info, const std::string &description);

    /**
     * Returns a BooleanElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    BooleanElement parse_boolean_element(const InstanceInfo& info, const std::string &description);

    /**
     * Returns a NumericalElement that counts the number of concepts.
     */
    NumericalElement make_count_element(const InstanceInfo& info, const ConceptElement& element);

    /**
     * Returns a NumericalElement that counts the number of roles.
     */
    NumericalElement make_count_element(const InstanceInfo& info, const RoleElement& element);

    /**
     * Returns a ConceptElement that counts the number of primitive concepts.
     */
    ConceptElement make_primitive_concept_element(const InstanceInfo& info, const std::string& name, unsigned pos);
};


}
}

#endif
