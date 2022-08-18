#include "policy.h"

namespace dlplan::policy {

template <typename T>
Feature<T>::Feature(int index) : BaseFeature(index) { }

template class Feature<bool>;
template class Feature<int>;
}
