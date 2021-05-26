#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPT_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPT_H_

#include "element.h"


namespace dlp {
namespace core {
namespace element {

class ConceptElement : public Element<Concepts> {
public:
    ConceptElement() { }

    size_t size() const {
        return m_result.size();
    }
};

}
}
}

#endif
