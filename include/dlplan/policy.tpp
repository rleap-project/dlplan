#include "policy.h"

namespace dlplan::policy {

template <typename T>
Feature<T>::Feature(std::shared_ptr<const PolicyRoot> root, int index, core::Element<T> element)
  : m_root(root), m_index(index), m_element(element) { }

template <typename T>
Feature<T>::~Feature() { }

template <typename T>
void Feature<T>::reset_cached_source_evaluation() {
    m_cached_source = false;
}

template <typename T>
void Feature<T>::reset_cached_target_evaluation() {
    m_cached_target = false;
}

template <typename T>
T Feature<T>::get_source_evaluation(const core::State& source) const {
    if (!m_cached_source) {
        m_source_evaluation = m_element.evaluate(source);
        m_cached_source = true;
    }
    return m_source_evaluation;
}

template <typename T>
T Feature<T>::get_target_evaluation(const core::State& target) const {
    if (!m_cached_target) {
        m_target_evaluation = m_element.evaluate(source);
        m_cached_target = true;
    }
    return m_target_evaluation;
}

template <typename T>
int Feature<T>::get_index() const { return m_index; }

template <typename T>
std::string Feature<T>::str() const { return std::to_string(m_index); }

template <typename T>
std::shared_ptr<const PolicyRoot> Feature<T>::get_root() const { return m_root; }

template class Feature<bool>;
template class Feature<int>;
}
