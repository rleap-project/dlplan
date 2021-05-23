#ifndef DLP_SRC_CORE_ELEMENTS_NUMERICAL_COUNT_H_
#define DLP_SRC_CORE_ELEMENTS_NUMERICAL_COUNT_H_

#include "../numerical.h"
#include "../concept.h"
#include "../role.h"
#include "../../types.h"

namespace dlp {
namespace core {

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

    virtual unsigned complexity() const override {
        return m_element->complexity() + 1;
    }
};

}
}

#endif