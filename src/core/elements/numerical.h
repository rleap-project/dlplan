#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_H_

#include "element.h"


namespace dlplan::core::element {

class Numerical : public Element<int> {
public:
    Numerical(const VocabularyInfo& vocabulary, const std::string& name)
    : Element<int>(vocabulary, name) { }
    ~Numerical() override = default;
};

}

#endif
