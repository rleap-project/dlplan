#include "../../include/dlplan/core.h"

#include <sstream>


namespace dlplan::core {

BaseElement::BaseElement(std::shared_ptr<const VocabularyInfo> vocabulary_info, bool is_static)
    : m_vocabulary_info(vocabulary_info), m_index(-1), m_is_static(is_static) { }

BaseElement::~BaseElement() { }

std::string BaseElement::compute_repr() const {
    std::stringstream ss;
    compute_repr(ss);
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const BaseElement& element) {
    os << element.compute_repr();
    return os;
}

std::string BaseElement::str() const {
    return compute_repr();
}

void BaseElement::set_index(ElementIndex index) {
    m_index = index;
}

ElementIndex BaseElement::get_index() const {
    return m_index;
}

std::shared_ptr<const VocabularyInfo> BaseElement::get_vocabulary_info() const {
    return m_vocabulary_info;
}

bool BaseElement::is_static() const {
    return m_is_static;
}

}
