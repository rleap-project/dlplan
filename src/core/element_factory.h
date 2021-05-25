#ifndef DLP_SRC_CORE_ELEMENT_FACTORY_H_
#define DLP_SRC_CORE_ELEMENT_FACTORY_H_

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "task_info.h"
#include "cache.h"
#include "types.h"
#include "elements/types.h"


namespace dlp {
namespace core {

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

    int add_atom(const std::string &predicate_name, const Name_Vec &object_names, bool goal);

    /**
     * Returns a pointer to a ConceptElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    element::ConceptElement_Ptr make_concept_element(const std::string &description);

    /**
     * Returns a pointer to a RoleElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    element::RoleElement_Ptr make_role_element(const std::string &description);

    /**
     * Returns a pointer to a NumericalElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    element::NumericalElement_Ptr make_numerical_element(const std::string &description);

    /**
     * Returns a pointer to a BooleanElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    element::BooleanElement_Ptr make_boolean_element(const std::string &description);
};

}
}

#endif
