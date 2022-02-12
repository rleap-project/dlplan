#include "core.h"

namespace dlplan::core {

template<typename T>
Element<T>::Element(
    std::shared_ptr<const VocabularyInfo> vocabulary_info)
    : m_vocabulary_info(vocabulary_info) { }

template<typename T>
Element<T>::~Element() { }

template<typename T>
std::shared_ptr<const VocabularyInfo> Element<T>::get_vocabulary_info() const {
    return m_vocabulary_info;
}

template class Element<dynamic_bitset::DynamicBitset<unsigned int>>;
template class Element<int>;
template class Element<bool>;

}