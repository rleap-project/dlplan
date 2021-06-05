#ifndef DLP_SRC_CORE_ELEMENTS_ELEMENT_ROOT_H_
#define DLP_SRC_CORE_ELEMENTS_ELEMENT_ROOT_H_

#include "element.h"

#include <iostream>


namespace dlp {
namespace core {
namespace element {

/**
 * The ElementRoot stores information from which the underlying Element was constructed.
 */
template<typename T>
class ElementRoot {
protected:
    const VocabularyInfoImpl* m_vocabulary_info;
    std::shared_ptr<Element<T>> m_element;

public:
    ElementRoot(const VocabularyInfoImpl& vocabulary_info, std::shared_ptr<Element<T>>&& element)
    : m_vocabulary_info(&vocabulary_info), m_element(std::move(element)) {
        if (!m_element) {
            throw std::runtime_error("ElementRoot::ElementRoot - tried to construct ElementRoot from nullptr");
        }
    }
    virtual ~ElementRoot() = default;

    /**
     * Evaluate and cache the last result.
     */
    const T& evaluate(const State& state) {
        if (state.get_instance_info()->get_vocabulary_info().get() != m_vocabulary_info) {
            throw std::runtime_error("ElementRoot::evaluate - mismatched vocabularies of Element and State.");
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
}

#endif
