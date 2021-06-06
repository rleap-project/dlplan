#ifndef DLP_SRC_CORE_ELEMENT_H_
#define DLP_SRC_CORE_ELEMENT_H_

#include "elements/element.h"


namespace dlp {
namespace core {

/**
 * The ElementRoot stores information from which the underlying Element was constructed.
 */
template<typename T>
class ElementImpl {
protected:
    const VocabularyInfoImpl* m_vocabulary_info;
    std::shared_ptr<element::Element<T>> m_element;

public:
    ElementImpl(const VocabularyInfoImpl& vocabulary_info, std::shared_ptr<element::Element<T>>&& element)
    : m_vocabulary_info(&vocabulary_info), m_element(std::move(element)) { }
    virtual ~ElementImpl() = default;

    /**
     * Evaluate and cache the last result.
     */
    const T& evaluate(const State& state) {
        if (state.get_instance_info()->get_vocabulary_info() != m_vocabulary_info) {
            throw std::runtime_error("ElementImpl::evaluate - mismatched vocabularies of Element and State.");
        }
        return m_element->evaluate(state);
    }

    unsigned compute_complexity() const {
        return m_element->compute_complexity();
    }

    std::string compute_repr() const {
        return m_element->compute_repr();
    }
};

}
}

#endif
