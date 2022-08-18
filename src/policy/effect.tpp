#include "effect.h"


namespace dlplan::policy {


template <typename T>
Effect<T>::Effect(std::shared_ptr<const Feature<T>> feature)
    : BaseEffect(feature), m_feature(feature) { }

template <typename T>
std::shared_ptr<const Feature<T>> Effect<T>::get_feature() const { return m_feature; }

template class Effect<bool>;
template class Effect<int>;

}
