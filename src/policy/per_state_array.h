#ifndef DLPLAN_SRC_POLICY_PER_STATE_ARRAY_H_
#define DLPLAN_SRC_POLICY_PER_STATE_ARRAY_H_

/**
 * Taken from fast-downward.org
 */

#include <vector>

#include "../utils/segmented_array_vector.h"
#include "../../include/dlplan/policy.h"


namespace dlplan::policy {

template<class T>
class ArrayView {
    T *p;
    int size_;
public:
    ArrayView(T *p, int size) : p(p), size_(size) {}
    ArrayView(const ArrayView<T> &other) = default;

    ArrayView<T> &operator=(const ArrayView<T> &other) = default;

    T &operator[](int index) {
        assert(index >= 0 && index < size_);
        return p[index];
    }

    const T &operator[](int index) const {
        assert(index >= 0 && index < size_);
        return p[index];
    }

    int size() const {
        return size_;
    }
};


template<class Element>
class PerStateArray {
private:
    const std::vector<Element> m_default_array;

    mutable utils::SegmentedArrayVector<Element> m_entries;

public:
    explicit PerStateArray(const std::vector<Element>& default_array)
      : m_default_array(default_array),
        m_entries(default_array.size()) { }

    ArrayView<Element> operator[](const core::State& state) {
        size_t state_id = state.get_index();
        // state_id does not fit anymore so we must resize
        if (m_entries.size() <= state_id) {
            m_entries.resize(state_id + 1, m_default_array.data());
        }
        return ArrayView<Element>(m_entries[state_id], m_default_array.size());
    }
};

}

#endif
