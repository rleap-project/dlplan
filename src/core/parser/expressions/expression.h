#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_EXPRESSION_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_EXPRESSION_H_

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "../../elements/types.h"
#include "../../cache.h"
#include "../../instance_info.h"


namespace dlp {
namespace core {
namespace parser {

/**
 * An Expression represents a node in an abstract syntax tree.
 */
class Expression {
protected:
    std::string m_name;
    std::vector<std::unique_ptr<Expression>> m_children;

public:
    Expression(const std::string &name,
        std::vector<std::unique_ptr<Expression>> &&children)
        : m_name(name), m_children(std::move(children)) {
    }

    /**
     * Returns true if the expression is a leaf, i.e.,
     * an expression that represents a position in a predicate for projection.
     */
    bool is_leaf() const {
        return m_children.empty();
    }

    /**
     * Getters.
     */
    const std::string& name() const {
        return m_name;
    }

    // Note: since children are stored as std::vector<Expression>
    // we do not know what type the children will be
    // and each concrete type must decide what method to call
    // in order to parse into the desired type.
    // As a result we cannot change this into a template.

    /**
     * Tries to parse the Expression into a ConceptElement.
     * If unsuccessful print human readable error messages and throw and exception.
     */
    virtual element::ConceptElement_Ptr parse_concept_element(std::shared_ptr<InstanceInfoImpl> parent, ElementCache &cache) const {
        return nullptr;
    }

    /**
     * Tries to parse the Expression into a RoleElement.
     * If unsuccessful print human readable error messages and throw and exception.
     */
    virtual element::RoleElement_Ptr parse_role_element(std::shared_ptr<InstanceInfoImpl> parent, ElementCache &cache) const {
        return nullptr;
    }

    /**
     * Tries to parse the Expression into a NumericalElement.
     * If unsuccessful print human readable error messages and throw and exception.
     */
    virtual element::NumericalElement_Ptr parse_numerical_element(std::shared_ptr<InstanceInfoImpl> parent, ElementCache &cache) const {
        return nullptr;
    }

    /**
     * Tries to parse the Expression into a BooleanElement.
     * If unsuccessful print human readable error messages and throw and exception.
     */
    virtual element::BooleanElement_Ptr parse_boolean_element(std::shared_ptr<InstanceInfoImpl> parent, ElementCache &cache) const {
        return nullptr;
    }
};

}
}
}

#endif
