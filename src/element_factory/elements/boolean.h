#ifndef DLP_SRC_ELEMENT_FACTORY_ELEMENTS_BOOLEAN_H_
#define DLP_SRC_ELEMENT_FACTORY_ELEMENTS_BOOLEAN_H_

#include "../element.h"
#include "../types.h"

namespace dlp {

class BooleanElement : public Element<bool> {
public:
    BooleanElement(std::shared_ptr<TaskInfo> task_info, bool goal) : Element<bool>(task_info, goal) { }

    size_t size() const override {
        return static_cast<size_t>(m_result);
    }
};

}

#endif
