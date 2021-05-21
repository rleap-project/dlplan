#ifndef DLP_SRC_ELEMENT_FACTORY_ROLE_ELEMENTS_ROLE_ELEMENT_H_
#define DLP_SRC_ELEMENT_FACTORY_ROLE_ELEMENTS_ROLE_ELEMENT_H_

#include <string>

#include "../element.h"

namespace dlp {

class RoleElement : public Element {
public:
    RoleElement();

    virtual std::size_t hash_value() const override;
};

}

#endif
