#ifndef DLP_SRC_CORE_ELEMENT_H_
#define DLP_SRC_CORE_ELEMENT_H_

#include "elements/element.h"


namespace dlp::core {

/**
 * The ElementRoot stores information from which the underlying Element was constructed.
 */
template<typename T>
class ElementImpl {
protected:
    const VocabularyInfo* m_vocabulary_info;
    std::shared_ptr<element::Element<T>> m_element;
    T m_result;

public:
    ElementImpl(const VocabularyInfo& vocabulary_info, std::shared_ptr<element::Element<T>>&& element)
    : m_vocabulary_info(&vocabulary_info), m_element(std::move(element)) { }
    virtual ~ElementImpl() = default;

    /**
     * Evaluate and cache the last result.
     */
    virtual const T& evaluate(const State& state) = 0;

    int compute_complexity() const {
        return m_element->compute_complexity();
    }

    std::string compute_repr() const {
        return m_element->compute_repr();
    }
};

}

#endif
