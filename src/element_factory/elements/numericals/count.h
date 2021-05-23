#ifndef DLP_SRC_ELEMENT_FACTORY_ELEMENTS_NUMERICAL_COUNT_H_
#define DLP_SRC_ELEMENT_FACTORY_ELEMENTS_NUMERICAL_COUNT_H_

#include "../numerical.h"
#include "../concept.h"
#include "../role.h"
#include "../../types.h"

namespace dlp {

template<typename T>
class CountNumericalElement : public NumericalElement {
protected:
    T m_element;

protected:
    virtual int evaluate_impl(const Index_Vec& atoms) override {
        return m_element->size();
    }

public:
    CountNumericalElement(std::shared_ptr<TaskInfo> task_info, bool goal, T element)
    : NumericalElement(task_info, goal), m_element(element) {}
};

}

#endif