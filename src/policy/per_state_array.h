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

template<class T>
class ConstArrayView {
    const T *p;
    const int size_;
public:
    ConstArrayView(T *p, int size) : p(p), size_(size) {}
    ConstArrayView(const ArrayView<T> &other) = delete;

    ConstArrayView<T> &operator=(const ArrayView<T> &other) = delete;

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
    const std::vector<Element> m_default_value;

    std::vector<utils::SegmentedArrayVector<Element>> m_entries;

public:
    PerStateArray(const std::vector<Element>& default_value)
      : m_default_value(default_value) { }

    ArrayView<Element> operator[](const State &state) {
        int state_id = state.get_index();
        if (m_entries->size() < state_id) {
            m_entries->resize(state_id, m_default_value.data());
        }
        return ArrayView<Element>(m_entries[state_id], m_default_value.size());
    }

    ArrayView<Element> operator[](const State &state) const {
        int state_id = state.get_index();
        int num_entries = m_entries->size();
        if (state_id >= num_entries) {
            return m_default_value;
        }
        return ConstArrayView<Element>(m_entries[state.get_index()], m_default_value.size());
    }
};

}

#endif
