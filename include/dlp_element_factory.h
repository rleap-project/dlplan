#ifndef DLP_INCLUDE_DLP_ELEMENTS_H_
#define DLP_INCLUDE_DLP_ELEMENTS_H_

#include <memory>

#include "../src/elements/element.h"

namespace dlp {

class ElementFactory {
public:
    ElementFactory();

    std::shared_ptr<Element> make_element();
};

}

#endif
