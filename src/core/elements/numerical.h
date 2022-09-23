#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_H_

#include "element.h"


namespace dlplan::core::element {

class Numerical : public Element<int> {
public:
    explicit Numerical(const VocabularyInfo& vocabulary) : Element<int>(vocabulary) { }
    ~Numerical() override = default;

    virtual int evaluate(const State& state) const = 0;
    virtual int evaluate(const State& state, DenotationsCaches& caches) const = 0;
    virtual std::vector<int>* evaluate(const States& states, DenotationsCaches& caches) const = 0;
};

}

#endif
