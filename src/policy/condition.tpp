#include "condition.h"


namespace dlplan::policy {

template <typename T>
Condition<T>::Condition(std::shared_ptr<const Feature<T>> feature) : BaseCondition(), m_feature(feature) { }

template <typename T>
std::shared_ptr<const Feature<T>> Condition<T>::get_feature() const { return m_feature; }

template class Condition<bool>;
template class Condition<int>;

}
