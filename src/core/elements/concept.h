#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPT_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPT_H_

#include "element.h"


namespace dlp {
namespace core {
namespace element {

class ConceptElement : public Element<Concepts> {
public:
    ConceptElement(std::shared_ptr<TaskInfo> task_info, bool goal) : Element<Concepts>(task_info, goal) { }

    size_t size() const override {
        return m_result.size();
    }
};

}
}
}

#endif
