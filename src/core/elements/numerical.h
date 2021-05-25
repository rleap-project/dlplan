#ifndef DLP_SRC_CORE_ELEMENTS_NUMERICAL_H_
#define DLP_SRC_CORE_ELEMENTS_NUMERICAL_H_

#include "element.h"


namespace dlp {
namespace core {
namespace element {

class NumericalElement : public Element<int> {
public:
    NumericalElement(bool goal) : Element<int>(goal) { }
};

}
}
}

#endif
