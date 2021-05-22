#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_EXPRESSION_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_EXPRESSION_H_

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "../types.h"
#include "../task_info.h"
#include "../cache.h"


namespace dlp {
namespace lisp {

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

    /**
     * Returns the canonical string representation of the expression.
     */
    std::string str() const {
        std::stringstream ss;
        for (const auto &child : m_children) {
            ss << child->str();
        }
        return ss.str();
    }

    /**
     * Tries to parse the Expression into a ConceptElement.
     * If unsuccessful print human readable error messages and throw and exception.
     */
    virtual ConceptElement_Ptr make_concept_element(const TaskInfo& task_info, ElementCache &cache) const {
        return nullptr;
    }

    /**
     * Tries to parse the Expression into a RoleElement.
     * If unsuccessful print human readable error messages and throw and exception.
     */
    virtual RoleElement_Ptr make_role_element(const TaskInfo& task_info, ElementCache &cache) const {
        return nullptr;
    }

    /**
     * Tries to parse the Expression into a NumericalElement.
     * If unsuccessful print human readable error messages and throw and exception.
     */
    virtual NumericalElement_Ptr make_numerical_element(const TaskInfo& task_info, ElementCache &cache) const {
        return nullptr;
    }

    /**
     * Tries to parse the Expression into a BooleanElement.
     * If unsuccessful print human readable error messages and throw and exception.
     */
    virtual BooleanElement_Ptr make_boolean_element(const TaskInfo& task_info, ElementCache &cache) const {
        return nullptr;
    }
};

}
}

#endif
