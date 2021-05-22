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
    TaskInfo m_task_info;

    ElementCache m_cache;

public:
    ElementFactoryImpl();

    void add_atom(const std::string &predicate_name,
        Name_Vec &object_names,
        bool constant=false);

    void add_atom(const std::string &predicate_name,
        unsigned predicate_idx,
        Name_Vec &object_names,
        Index_Vec &object_idxs,
        bool constant=false);

    ConceptElement_Ptr make_concept_element(const std::string &description);

    RoleElement_Ptr make_role_element(const std::string &description);

    NumericalElement_Ptr make_numerical_element(const std::string &description);

    BooleanElement_Ptr make_boolean_element(const std::string &description);
};

}

#endif
