#ifndef DLP_INCLUDE_DLP_ELEMENT_FACTORY_H_
#define DLP_INCLUDE_DLP_ELEMENT_FACTORY_H_

#include <memory>
#include <string>
#include <vector>

#include "../../src/element_factory/concept_elements/concept_element.h"
#include "../../src/element_factory/role_elements/role_element.h"
#include "../../src/element_factory/cache.h"
#include "../../src/element_factory/types.h"


namespace dlp {

/**
 * ElementFactory provides functionality for constructing
 * nested composites of description logics elements
 * that can be evaluated on state of planning instances.
 */
class ElementFactory {
private:
    // TODO(dominik): The parser must transform descriptions into canonical representation.
    Cache<std::string, Element_Ptr> m_element_cache;
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
};

}

#endif
