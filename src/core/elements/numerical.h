#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_H_

#include "element.h"


namespace dlplan::core::element {

class Numerical : public Element<int> {
public:
    explicit Numerical(const VocabularyInfo& vocabulary) : Element<int>(vocabulary) { }
    ~Numerical() override = default;
};

}

#endif
