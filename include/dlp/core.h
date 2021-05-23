#ifndef DLP_INCLUDE_DLP_CORE_H_
#define DLP_INCLUDE_DLP_CORE_H_

#include "../../src/core/element_factory.h"


namespace dlp {
namespace core {
class ElementFactory;

/**
 * Abstract base class of any Element.
 */
template<typename T>
class Element {
public:
    /**
     * Evaluates the element of a state given as a vector of atom indices.
     */
    virtual T evaluate(const Index_Vec& atoms) const = 0;

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
    ConceptElement_Ptr m_pImpl;

public:
    ConceptElement(ElementFactory& factory, const std::string& description);

    virtual Concepts evaluate(const Index_Vec& atoms) const override;

    virtual unsigned complexity() const override;
};

/**
 * ConceptElement evaluates to Roles.
 */
class RoleElement : public Element<Roles> {
protected:
    RoleElement_Ptr m_pImpl;

public:
    RoleElement(ElementFactory& factory, const std::string& description);

    virtual Roles evaluate(const Index_Vec& atoms) const override;

    virtual unsigned complexity() const override;
};

/**
 * NumericalElement evaluates to int.
 */
class NumericalElement : public Element<int> {
protected:
    NumericalElement_Ptr m_pImpl;

public:
    NumericalElement(ElementFactory& factory, const std::string& description);

    virtual int evaluate(const Index_Vec& atoms) const override;

    virtual unsigned complexity() const override;
};

/**
 * BooleanElement evaluates to bool.
 */
class BooleanElement : public Element<bool> {
protected:
    BooleanElement_Ptr m_pImpl;

public:
    BooleanElement(ElementFactory& factory, const std::string& description);

    virtual bool evaluate(const Index_Vec& atoms) const override;

    virtual unsigned complexity() const override;
};


/**
 * The FeatureFactory stores elements and takes care of uniqueness.
 */
class ElementFactory {
private:
    std::unique_ptr<ElementFactoryImpl> m_pImpl;

private:
    /**
     * Returns a pointer to a ConceptElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    ConceptElement_Ptr make_concept_element(const std::string &description);

    /**
     * Returns a pointer to a RoleElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    RoleElement_Ptr make_role_element(const std::string &description);

    /**
     * Returns a pointer to a NumericalElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    NumericalElement_Ptr make_numerical_element(const std::string &description);

    /**
     * Returns a pointer to a BooleanElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    BooleanElement_Ptr make_boolean_element(const std::string &description);

    /**
     * Private methods needed during construction.
     */
    friend class ConceptElement;
    friend class RoleElement;
    friend class NumericalElement;
    friend class BooleanElement;

public:
    ElementFactory();

    /**
     * Methods for initializing task information.
     */
    void add_atom(const std::string &predicate_name, const Name_Vec &object_names);
    void set_constant_atoms(const Index_Vec& constant_atom_idxs);
    void set_goal_atoms(const Index_Vec& goal_atom_idxs);
};

}
}

#endif
