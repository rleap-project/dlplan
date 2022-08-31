#include "core.h"

namespace dlplan::core {

template<typename T>
Element<T>::Element(
    std::shared_ptr<const VocabularyInfo> vocabulary_info, int index)
    : BaseElement(vocabulary_info, index) { }

template<typename T>
Element<T>::~Element() { }

template class Element<utils::DynamicBitset<unsigned int>>;
template class Element<int>;
template class Element<bool>;

}