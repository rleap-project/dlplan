#ifndef DLP_SRC_CORE_ELEMENTS_BOOLEAN_H_
#define DLP_SRC_CORE_ELEMENTS_BOOLEAN_H_

#include "element.h"


namespace dlp {
namespace core {
namespace element {

class BooleanElement : public Element<bool> {
public:
    BooleanElement(std::shared_ptr<InstanceInfoImpl> parent, const std::string& name) : Element<bool>(parent, name) { }
};

}
}
}

#endif
