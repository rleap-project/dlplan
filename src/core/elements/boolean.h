#ifndef DLP_SRC_CORE_ELEMENTS_BOOLEAN_H_
#define DLP_SRC_CORE_ELEMENTS_BOOLEAN_H_

#include "element.h"


namespace dlp {
namespace core {
namespace element {

class Boolean : public Element<bool> {
public:
    Boolean(const VocabularyInfoImpl& vocabulary, const std::string& name) : Element<bool>(vocabulary, name) { }
};

}
}
}

#endif
