#ifndef DLP_SRC_CORE_ROLE_H_
#define DLP_SRC_CORE_ROLE_H_

#include "element.h"


namespace dlp {
namespace core {

/**
 * The ElementRoot stores information from which the underlying Element was constructed.
 */
class RoleImpl : public ElementImpl<element::RoleDenotation> {
public:
    RoleImpl(const VocabularyInfoImpl& vocabulary_info, std::shared_ptr<element::Element<element::RoleDenotation>>&& element)
    : ElementImpl<element::RoleDenotation>(vocabulary_info, std::move(element)) {
        if (!m_element) {
            throw std::runtime_error("RoleImpl::RoleImpl - tried to construct ConceptImpl from nullptr");
        }
    }
    virtual ~RoleImpl() = default;
};

}
}

#endif
