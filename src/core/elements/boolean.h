#ifndef DLP_SRC_CORE_ELEMENTS_BOOLEAN_H_
#define DLP_SRC_CORE_ELEMENTS_BOOLEAN_H_

#include "element.h"


namespace dlp {
namespace core {
namespace element {

class BooleanElement : public Element<bool> {
public:
    BooleanElement(std::shared_ptr<TaskInfo> task_info, bool goal) : Element<bool>(task_info, goal) { }

    size_t size() const override {
        return static_cast<size_t>(m_result);
    }
};

}
}
}

#endif
