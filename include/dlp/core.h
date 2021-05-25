#ifndef DLP_INCLUDE_DLP_CORE_H_
#define DLP_INCLUDE_DLP_CORE_H_

#include "../../src/core/element_factory.h"


namespace dlp {
namespace core {
class ElementFactory;

using Concept = int;
using Concepts = std::vector<Concept>;

using Role = std::pair<Concept, Concept>;
using Roles = std::vector<Role>;

using Name_Vec = std::vector<std::string>;
using Index_Vec = std::vector<int>;

/**
 * Abstract base class of any Element.
 */
template<typename T>
class Element {
public:
    /**
     * Evaluates the element for a state given as a vector of atom indices.
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
    element::ConceptElement_Ptr m_pImpl;

public:
    ConceptElement(element::ConceptElement_Ptr pImpl);

    virtual Concepts evaluate(const Index_Vec& atoms) const override;

    virtual unsigned complexity() const override;
};

/**
 * ConceptElement evaluates to Roles.
 */
class RoleElement : public Element<Roles> {
protected:
    element::RoleElement_Ptr m_pImpl;

public:
    RoleElement(element::RoleElement_Ptr pImpl);

    virtual Roles evaluate(const Index_Vec& atoms) const override;

    virtual unsigned complexity() const override;
};

/**
 * NumericalElement evaluates to int.
 */
class NumericalElement : public Element<int> {
protected:
    element::NumericalElement_Ptr m_pImpl;

public:
    NumericalElement(element::NumericalElement_Ptr pImpl);

    virtual int evaluate(const Index_Vec& atoms) const override;

    virtual unsigned complexity() const override;
};

/**
 * BooleanElement evaluates to bool.
 */
class BooleanElement : public Element<bool> {
protected:
    element::BooleanElement_Ptr m_pImpl;

public:
    BooleanElement(element::BooleanElement_Ptr pImpl);

    virtual bool evaluate(const Index_Vec& atoms) const override;

    virtual unsigned complexity() const override;
};


/**
 * The FeatureFactory stores elements and takes care of uniqueness.
 */
class ElementFactory {
private:
    std::unique_ptr<ElementFactoryImpl> m_pImpl;

public:
    ElementFactory();

    /**
     * Methods for initializing task information.
     */
    int add_atom(const std::string &predicate_name, const Name_Vec &object_names, bool goal);

    /**
     * Returns a ConceptElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    ConceptElement make_concept_element(const std::string &description);

    /**
     * Returns a RoleElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    RoleElement make_role_element(const std::string &description);

    /**
     * Returns a NumericalElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    NumericalElement make_numerical_element(const std::string &description);

    /**
     * Returns a BooleanElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    BooleanElement make_boolean_element(const std::string &description);
};

}
}

#endif
