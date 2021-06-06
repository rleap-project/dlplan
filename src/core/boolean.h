#ifndef DLP_SRC_CORE_BOOLEAN_H_
#define DLP_SRC_CORE_BOOLEAN_H_

#include "element.h"


namespace dlp {
namespace core {

/**
 * The ElementRoot stores information from which the underlying Element was constructed.
 */
class BooleanImpl : public ElementImpl<bool> {
public:
    BooleanImpl(const VocabularyInfoImpl& vocabulary_info, std::shared_ptr<element::Element<bool>>&& element)
    : ElementImpl<bool>(vocabulary_info, std::move(element)) {
        if (!m_element) {
            throw std::runtime_error("BooleanImpl::BooleanImpl - tried to construct ConceptImpl from nullptr");
        }
    }
    virtual ~BooleanImpl() = default;
};

}
}

#endif
