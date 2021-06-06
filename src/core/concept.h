#ifndef DLP_SRC_CORE_CONCEPT_H_
#define DLP_SRC_CORE_CONCEPT_H_

#include "element.h"


namespace dlp {
namespace core {

/**
 * The ElementRoot stores information from which the underlying Element was constructed.
 */
class ConceptImpl : public ElementImpl<element::ConceptDenotation> {
public:
    ConceptImpl(const VocabularyInfoImpl& vocabulary_info, std::shared_ptr<element::Element<element::ConceptDenotation>>&& element)
    : ElementImpl<element::ConceptDenotation>(vocabulary_info, std::move(element)) {
        if (!m_element) {
            throw std::runtime_error("ConceptImpl::ConceptImpl - tried to construct ConceptImpl from nullptr");
        }
    }
    virtual ~ConceptImpl() = default;
};

}
}

#endif
