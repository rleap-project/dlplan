#ifndef DLP_SRC_ELEMENT_FACTORY_LISP_AST_H_
#define DLP_SRC_ELEMENT_FACTORY_LISP_AST_H_

#include <memory>
#include <vector>

namespace dlp {
namespace lisp {

class AST {
private:
    std::vector<std::shared_ptr<AST>> m_children;

public:
    AST();

    std::string str() const;
};

}
}

#endif
