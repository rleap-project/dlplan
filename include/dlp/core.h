#ifndef DLP_INCLUDE_DLP_CORE_H_
#define DLP_INCLUDE_DLP_CORE_H_

#include "../../src/core/element_factory.h"
#include "../../src/core/instance_info.h"


namespace dlp {
namespace core {
class ElementFactory;

using Concept = int;
using Concepts = std::vector<Concept>;

using Role = std::pair<Concept, Concept>;
using Roles = std::vector<Role>;

using Name_Vec = std::vector<std::string>;
using Index_Vec = std::vector<int>;


class InstanceInfo {
protected:
    std::shared_ptr<InstanceInfoImpl> m_pImpl;

    friend class ElementFactory;
    friend class ConceptElement;
    friend class RoleElement;
    friend class NumericalElement;
    friend class BooleanElement;

public:
    InstanceInfo();

    /**
     * Methods for initializing instance information.
     */
    int add_atom(const std::string &predicate_name, const Name_Vec &object_names, bool goal);
};

/**
 * Abstract base class of any Element.
 */
template<typename T>
class Element {
protected:
    /**
     * During evaluation we only allow using the instance information
     * that was used during the construction.
     * To use a different instance information for the same Element
     * one has to construct the element by passing the specific instance information
     * to the construction method in the ElementFactory.
     * We set this requirement because during the construction process
     * we assert the existence of respective predicates and their arities.
     */
    InstanceInfo m_info;

public:
    Element(const InstanceInfo& info) : m_info(info) { }

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
    ConceptElement(const InstanceInfo& info, element::ConceptElement_Ptr pImpl);

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
    RoleElement(const InstanceInfo& info, element::RoleElement_Ptr pImpl);

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
    NumericalElement(const InstanceInfo& info, element::NumericalElement_Ptr pImpl);

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
    BooleanElement(const InstanceInfo& info, element::BooleanElement_Ptr pImpl);

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
     * Returns a ConceptElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    ConceptElement make_concept_element(const InstanceInfo& info, const std::string &description);

    /**
     * Returns a RoleElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    RoleElement make_role_element(const InstanceInfo& info, const std::string &description);

    /**
     * Returns a NumericalElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    NumericalElement make_numerical_element(const InstanceInfo& info, const std::string &description);

    /**
     * Returns a BooleanElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    BooleanElement make_boolean_element(const InstanceInfo& info, const std::string &description);
};


}
}

#endif
