#ifndef DLP_SRC_CORE_BOOLEAN_H_
#define DLP_SRC_CORE_BOOLEAN_H_

#include "element.h"


namespace dlp {
namespace core {

class BooleanImpl : public ElementImpl<bool> {
public:
    BooleanImpl(const VocabularyInfo& vocabulary_info, std::shared_ptr<element::Element<bool>>&& element)
    : ElementImpl<bool>(vocabulary_info, std::move(element)) {
        if (!m_element) {
            throw std::runtime_error("BooleanImpl::BooleanImpl - tried to construct Boolean from nullptr");
        }
    }
    ~BooleanImpl() override = default;

    const bool& evaluate(const State& state) override {
        if (state.get_instance_info()->get_vocabulary_info().get() != m_vocabulary_info) {
            throw std::runtime_error("BooleanImpl::evaluate - mismatched vocabularies of Boolean and State.");
        }
        return m_element->evaluate(state);
    }
};

}
}

#endif
