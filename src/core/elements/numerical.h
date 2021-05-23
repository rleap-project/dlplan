#ifndef DLP_SRC_ELEMENT_FACTORY_ELEMENTS_NUMERICAL_H_
#define DLP_SRC_ELEMENT_FACTORY_ELEMENTS_NUMERICAL_H_

#include "../element.h"

namespace dlp {

class NumericalElement : public Element<int> {
public:
    NumericalElement(std::shared_ptr<TaskInfo> task_info, bool goal) : Element<int>(task_info, goal) { }

    size_t size() const override {
        return static_cast<size_t>(m_result);
    }
};

}

#endif
