#include "../../include/dlp/element_factory.h"
#include "lisp/parser.h"

namespace dlp {

ElementFactory::ElementFactory() {
}

ConceptElement_Ptr ElementFactory::make_concept_element(const std::string &description) {
    lisp::AST ast = lisp::Parser().parse(description);
    return std::make_shared<ConceptElement>(ConceptElement());
}

RoleElement_Ptr ElementFactory::make_role_element(const std::string &description) {
    return std::make_shared<RoleElement>(RoleElement());
}

}
