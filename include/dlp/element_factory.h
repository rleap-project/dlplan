#ifndef DLP_INCLUDE_DLP_ELEMENT_FACTORY_H_
#define DLP_INCLUDE_DLP_ELEMENT_FACTORY_H_

#include "../../src/element_factory/types.h"
#include "../../src/element_factory/element_factory_impl.h"
#include <memory>

namespace dlp {

/**
 * The FeatureFactory allow the construction of ConceptElement,
 * RoleElement, NumericalElement, and BooleanElement from text.
 */
class ElementFactory {
private:
    std::unique_ptr<ElementFactoryImpl> m_pImpl;

public:
    ElementFactory();

    void add_atom(const std::string &predicate_name,
        Name_Vec &object_names,
        bool constant=false);

    void add_atom(const std::string &predicate_name,
        unsigned predicate_idx,
        Name_Vec &object_names,
        Index_Vec &object_idxs,
        bool constant=false);

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
};

}

#endif
