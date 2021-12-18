#include "../../include/dlplan/core.h"

namespace dlplan::core {

template<typename T>
Element<T>::Element(
    std::shared_ptr<const VocabularyInfo> vocabulary_info,
    std::shared_ptr<const element::Element<T>> element)
    : m_vocabulary_info(vocabulary_info), m_element(element) { }

template<typename T>
Element<T>::Element(const Element<T>& other) : m_vocabulary_info(other.m_vocabulary_info), m_element(other.m_element) { }

template<typename T>
Element<T>& Element<T>::operator=(const Element<T>& other) {
    if (this != &other) {
        m_vocabulary_info = other.m_vocabulary_info;
        m_element = other.m_element;
    }
    return *this;
}

template<typename T>
Element<T>::~Element() { }

template<typename T>
std::shared_ptr<const VocabularyInfo> Element<T>::get_vocabulary_info() const {
    return m_vocabulary_info;
}

template<typename T>
std::shared_ptr<const element::Element<T>> Element<T>::get_element() const {
    return m_element;
}

template class Element<dynamic_bitset::DynamicBitset<unsigned int>>;
template class Element<int>;
template class Element<bool>;

}