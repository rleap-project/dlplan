#ifndef DLP_SRC_ELEMENT_FACTORY_ELEMENT_FACTORY_IMPL_H_
#define DLP_SRC_ELEMENT_FACTORY_ELEMENT_FACTORY_IMPL_H_

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "task_info.h"
#include "cache.h"
#include "types.h"


namespace dlp {

/**
 * ElementFactory provides functionality for constructing
 * nested composites of description logics elements
 * that can be evaluated on state of planning instances.
 */
class ElementFactoryImpl {
protected:
    std::shared_ptr<TaskInfo> m_task_info;

    ElementCache m_cache;

public:
    ElementFactoryImpl();

    void add_atom(const std::string &predicate_name, const Name_Vec &object_names);
    void set_constant_atoms(const Index_Vec& constant_atom_idxs);
    void set_goal_atoms(const Index_Vec& goal_atom_idxs);

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
