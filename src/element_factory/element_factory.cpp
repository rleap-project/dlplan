#include "../../include/dlp/element_factory.h"


namespace dlp {

ElementFactory::ElementFactory() {
}

std::shared_ptr<ConceptElement> ElementFactory::make_concept_element(const std::string &description) {
    return std::make_shared<ConceptElement>(ConceptElement());
}

std::shared_ptr<RoleElement> ElementFactory::make_role_element(const std::string &description) {
    return std::make_shared<RoleElement>(RoleElement());
}

}
