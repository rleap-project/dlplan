#ifndef DLP_SRC_CORE_ELEMENTS_NUMERICAL_H_
#define DLP_SRC_CORE_ELEMENTS_NUMERICAL_H_

#include "element.h"


namespace dlp {
namespace core {
namespace element {

class Numerical : public Element<int> {
public:
    Numerical(const VocabularyInfo& vocabulary, const std::string& name) : Element<int>(vocabulary, name) { }
};

}
}
}

#endif
