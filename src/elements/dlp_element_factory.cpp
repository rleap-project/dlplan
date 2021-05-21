#include "../../include/dlp_element_factory.h"


namespace dlp {

ElementFactory::ElementFactory() {
}

std::shared_ptr<Element> ElementFactory::make_element() {
    return std::make_shared<Element>(Element());
}

}
