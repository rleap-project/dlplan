#include "policy.h"

namespace dlplan::policy {

template <typename T>
Feature<T>::Feature(std::shared_ptr<const PolicyRoot> root, int index)
  : m_root(root), m_index(index) { }

template <typename T>
Feature<T>::~Feature() { }

template <typename T>
int Feature<T>::get_index() const { return m_index; }

template <typename T>
std::shared_ptr<const PolicyRoot> Feature<T>::get_root() const { return m_root; }

template class Feature<bool>;
template class Feature<int>;
}
