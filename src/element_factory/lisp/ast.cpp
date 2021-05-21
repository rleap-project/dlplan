#include "ast.h"

#include "tokenizer.h"

namespace dlp {
namespace lisp {

AST::AST(const std::string &name, std::vector<std::unique_ptr<AST>> &&children)
    : m_name(name), m_children(std::move(children)) {
}

bool AST::is_leaf() const {
    return m_children.empty();
}

const std::string& AST::name() const {
    return m_name;
}

std::string AST::str() const {
}


Number::Number(const std::string &name, std::vector<std::unique_ptr<AST>> &&children)
    : AST(name, std::move(children)), m_number(atoi(name.c_str())) {
}

}
}
