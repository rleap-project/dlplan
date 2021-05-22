#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_EXPRESSION_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_EXPRESSIONS_EXPRESSION_H_

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "../../types.h"

namespace dlp {
namespace lisp {

class Expression {
protected:
    std::string m_name;
    std::vector<std::unique_ptr<Expression>> m_children;

public:
    Expression(const std::string &name,
        std::vector<std::unique_ptr<Expression>> &&children)
        : m_name(name), m_children(std::move(children)) {
    }

    bool is_leaf() const {
        return m_children.empty();
    }
    const std::string& name() const {
        return m_name;
    }
    /**
     * Returns canonical string representation of expression.
     */
    std::string str() const {
        std::stringstream ss;
        for (const auto &child : m_children) {
            ss << child->str();
        }
        return ss.str();
    }

    // TODO(dominik): Use visitor pattern aka double dispatch for constructing respective element
    // change return type to concept
    virtual ConceptElement_Ptr make_concept(/* pass element factory as visitor using interace*/) const {
        std::cout << "Expression::make_concept - the expression is not implemented as concept." << std::endl;
        throw std::exception();
    }
    // change return type to role
    virtual RoleElement_Ptr make_role(/* pass element factory as visitor using interace*/) const {
        std::cout << "Expression::make_role - the expression is not implemented as role." << std::endl;
        throw std::exception();
    }
    // change return type to numerical feature
    virtual void make_numerical_feature(/* pass element factory as visitor using interace*/) const {
        std::cout << "Expression::make_numerical_feature - the expression is not implemented as numerical feature." << std::endl;
        throw std::exception();
    }
    // change return type to boolean feature
    virtual void make_boolean_feature(/* pass element factory as visitor using interace*/) const {
        std::cout << "Expression::make_boolean_feature - the expression is not implemented as boolean feature." << std::endl;
        throw std::exception();
    }
};

}
}

#endif
