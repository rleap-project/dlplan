#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_H_

#include "element.h"


namespace dlplan::core::element {

class Boolean : public Element<bool> {
public:
    explicit Boolean(const VocabularyInfo& vocabulary) : Element<bool>(vocabulary) { }
    ~Boolean() override = default;

    virtual bool evaluate(const State& state) const = 0;
    virtual DENOTS<bool>* evaluate(const States& states, DenotationsCaches& caches) const = 0;
};

}

#endif
