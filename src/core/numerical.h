#ifndef DLP_SRC_CORE_NUMERICAL_H_
#define DLP_SRC_CORE_NUMERICAL_H_

#include "element.h"


namespace dlp {
namespace core {

/**
 * The ElementRoot stores information from which the underlying Element was constructed.
 */
class NumericalImpl : public ElementImpl<int> {
public:
    NumericalImpl(const VocabularyInfo& vocabulary_info, std::shared_ptr<element::Element<int>>&& element)
    : ElementImpl<int>(vocabulary_info, std::move(element)) {
        if (!m_element) {
            throw std::runtime_error("NumericalImpl::NumericalImpl - tried to construct ConceptImpl from nullptr");
        }
    }
    virtual ~NumericalImpl() = default;
};

}
}

#endif
