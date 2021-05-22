#ifndef DLP_SRC_ELEMENT_FACTORY_ELEMENTS_NUMERICAL_H_
#define DLP_SRC_ELEMENT_FACTORY_ELEMENTS_NUMERICAL_H_

#include "../element.h"

namespace dlp {

class NumericalElement : public Element<int> {
public:
    NumericalElement(bool goal, const Index_Vec& goal_atoms)
    : Element<int>(goal, goal_atoms) { }

    size_t size() const override {
        return static_cast<size_t>(m_result);
    }
};

}

#endif
