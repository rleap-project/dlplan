#ifndef DLP_SRC_CORE_ELEMENT_FACTORY_H_
#define DLP_SRC_CORE_ELEMENT_FACTORY_H_

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "instance_info.h"
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
    ElementCache m_cache;

public:
    ElementFactoryImpl();

    /**
     * Returns a pointer to a ConceptElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    element::ConceptElement_Ptr parse_concept_element(const InstanceInfoImpl& info, const std::string &description);

    /**
     * Returns a pointer to a RoleElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    element::RoleElement_Ptr parse_role_element(const InstanceInfoImpl& info, const std::string &description);

    /**
     * Returns a pointer to a NumericalElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    element::NumericalElement_Ptr parse_numerical_element(const InstanceInfoImpl& info, const std::string &description);

    /**
     * Returns a pointer to a BooleanElement if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    element::BooleanElement_Ptr parse_boolean_element(const InstanceInfoImpl& info, const std::string &description);
};

}
}

#endif