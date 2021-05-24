#ifndef DLP_SRC_CORE_ELEMENTS_ROLE_H_
#define DLP_SRC_CORE_ELEMENTS_ROLE_H_

#include "element.h"


namespace dlp {
namespace core {
namespace element {

class RoleElement : public Element<Roles> {
public:
    RoleElement(std::shared_ptr<TaskInfo> task_info, bool goal) : Element<Roles>(task_info, goal) { }

    size_t size() const override {
        return m_result.size();
    }
};

}
}
}

#endif
