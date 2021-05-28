#ifndef DLP_SRC_CORE_ELEMENTS_BOOLEAN_H_
#define DLP_SRC_CORE_ELEMENTS_BOOLEAN_H_

#include "element.h"


namespace dlp {
namespace core {
namespace element {

class BooleanElement : public Element<bool> {
public:
    BooleanElement(const InstanceInfoImpl& info, const std::string& name) : Element<bool>(info, name) { }
};

}
}
}

#endif
