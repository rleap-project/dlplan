#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_AST_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_AST_H_

#include <memory>
#include <vector>

#include "types.h"

namespace dlp {
namespace lisp {

class AST {
private:
    std::string m_name;
    std::vector<std::unique_ptr<AST>> m_children;

public:
    AST(const std::string &name, std::vector<std::unique_ptr<AST>> &&children);

    bool is_leaf() const;

    const std::string& name() const;
    std::string str() const;

    // TODO(dominik): Use visitor pattern aka double dispatch for constructing respective element
};

class Number : public AST {
private:
    int m_number;
public:
    Number(const std::string &name, std::vector<std::unique_ptr<AST>> &&children);
};

class PrimitiveConceptElement : public AST {
    PrimitiveConceptElement(const std::string &name, std::vector<std::unique_ptr<AST>> &&children);
};

class PrimitiveRoleElement : public AST {
    PrimitiveRoleElement(const std::string &name, std::vector<std::unique_ptr<AST>> &&children);
};

// TODO(dominik): Use one class for each feature and element
class AndConceptElement : public AST {
    AndConceptElement(const std::string &name, std::vector<std::unique_ptr<AST>> &&children);
};

class OrConceptElement : public AST {
    OrConceptElement(const std::string &name, std::vector<std::unique_ptr<AST>> &&children);
};

// ...

class AndRoleElement : public AST {
    AndRoleElement(const std::string &name, std::vector<std::unique_ptr<AST>> &&children);
};

class OrRoleElement : public AST {
    OrRoleElement(const std::string &name, std::vector<std::unique_ptr<AST>> &&children);
};

// ...

class CountFeature : public AST {
    CountFeature(const std::string &name, std::vector<std::unique_ptr<AST>> &&children);
};

class EmptyFeature : public AST {
    EmptyFeature(const std::string &name, std::vector<std::unique_ptr<AST>> &&children);
};

class ConceptDistanceFeature : public AST {
    ConceptDistanceFeature(const std::string &name, std::vector<std::unique_ptr<AST>> &&children);
};

class SumConceptDistanceFeature : public AST {
    SumConceptDistanceFeature(const std::string &name, std::vector<std::unique_ptr<AST>> &&children);
};

class RoleDistanceFeature : public AST {
    RoleDistanceFeature(const std::string &name, std::vector<std::unique_ptr<AST>> &&children);
};

class SumRoleDistanceFeature : public AST {
    SumRoleDistanceFeature(const std::string &name, std::vector<std::unique_ptr<AST>> &&children);
};


}
}

#endif
