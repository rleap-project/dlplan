#include "policy.h"

namespace dlplan::policy {

template <typename T>
Feature<T>::Feature(std::shared_ptr<const PolicyRoot> root, int index)
  : BaseFeature(root, index) { }

template class Feature<bool>;
template class Feature<int>;
}
