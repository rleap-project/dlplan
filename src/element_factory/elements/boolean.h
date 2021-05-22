#ifndef DLP_SRC_ELEMENT_FACTORY_ELEMENTS_BOOLEAN_H_
#define DLP_SRC_ELEMENT_FACTORY_ELEMENTS_BOOLEAN_H_

#include "../element.h"

namespace dlp {

class BooleanElement : public Element<bool> {
public:
    BooleanElement(bool goal, const Index_Vec& goal_atoms)
    : Element<bool>(goal, goal_atoms) { }

    size_t size() const override {
        return static_cast<size_t>(m_result);
    }
};

}

#endif
