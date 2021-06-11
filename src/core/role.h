#ifndef DLP_SRC_CORE_ROLE_H_
#define DLP_SRC_CORE_ROLE_H_

#include <algorithm>

#include "element.h"


namespace dlp {
namespace core {

class RoleImpl : public ElementImpl<RoleDenotation> {
public:
    RoleImpl(const VocabularyInfo& vocabulary_info, std::shared_ptr<element::Element<RoleDenotation>>&& element)
    : ElementImpl<RoleDenotation>(vocabulary_info, std::move(element)) {
        if (!m_element) {
            throw std::runtime_error("RoleImpl::RoleImpl - tried to construct Role from nullptr");
        }
    }
    ~RoleImpl() override = default;

    const RoleDenotation& evaluate(const State& state) override {
        if (state.get_instance_info()->get_vocabulary_info().get() != m_vocabulary_info) {
            throw std::runtime_error("RoleImpl::evaluate - mismatched vocabularies of Role and State.");
        }
        m_result = m_element->evaluate(state);
        std::sort(m_result.begin(), m_result.end());
        return m_result;
    }
};

}
}

#endif
