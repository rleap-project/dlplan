#ifndef DLP_SRC_CORE_ELEMENTS_CONCEPT_H_
#define DLP_SRC_CORE_ELEMENTS_CONCEPT_H_

#include "element.h"


namespace dlp {
namespace core {
namespace element {

class Concept : public Element<ConceptDenotation> {
public:
    Concept(const VocabularyInfo& vocabulary, const std::string& name) : Element<ConceptDenotation>(vocabulary, name) { }

    size_t size() const {
        return m_result.size();
    }
};

}
}
}

#endif
