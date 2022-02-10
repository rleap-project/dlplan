#include "core.h"

namespace dlplan::core {

template<typename T>
Element<T>::Element(
    std::shared_ptr<const VocabularyInfo> vocabulary_info)
    : m_vocabulary_info(vocabulary_info) { }

template<typename T>
Element<T>::Element(const Element<T>& other) : m_vocabulary_info(other.m_vocabulary_info) { }

template<typename T>
Element<T>& Element<T>::operator=(const Element<T>& other) {
    if (this != &other) {
        m_vocabulary_info = other.m_vocabulary_info;
    }
    return *this;
}

template<typename T>
Element<T>::~Element() { }

template<typename T>
std::shared_ptr<const VocabularyInfo> Element<T>::get_vocabulary_info() const {
    return m_vocabulary_info;
}

template class Element<ConceptDenotation>;
template class Element<int>;
template class Element<bool>;

}