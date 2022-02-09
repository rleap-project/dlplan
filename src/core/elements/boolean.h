#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_H_

#include "element.h"


namespace dlplan::core::element {

class Boolean : public Element<bool> {
public:
    Boolean(const VocabularyInfo& vocabulary, const std::string& name)
    : Element<bool>(vocabulary, name) { }
    ~Boolean() override = default;
};

}

#endif
