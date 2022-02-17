#ifndef DLPLAN_INCLUDE_DLPLAN_PER_STATE_ARRAY_H_
#define DLPLAN_INCLUDE_DLPLAN_PER_STATE_ARRAY_H_

/**
 * Taken from fast-downward.org
 */

#include <cassert>
#include <vector>

#include "segmented_array_vector.h"


namespace dlplan::utils {

template<class T>
class ArrayView {
    T *p;
    size_t size_;
public:
    typedef T* iterator;
    typedef const T* const_iterator;

    ArrayView(T *p, size_t size) : p(p), size_(size) {}
    ArrayView(const ArrayView<T> &other) = default;

    ArrayView<T> &operator=(const ArrayView<T> &other) = default;

    T &operator[](int index) {
        assert(index >= 0 && index < static_cast<int>(size()));
        return p[index];
    }

    iterator begin() {
        return p;
    }

    const_iterator begin() const {
        return p;
    }

    iterator end() {
        return p + size_;
    }

    const_iterator end() const {
        return p + size_;
    }

    const T &back() const {
        assert(size_ > 0);
        return p[size_];
    }

    T &back() {
        assert(size_ > 0);
        return p[size_];
    }

    const T &operator[](int index) const {
        assert(index >= 0 && index < static_cast<int>(size()));
        return p[index];
    }

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return (size_ == 0);
    }
};


template<class Element>
class PerIndexArray {
private:
    std::vector<Element> m_default_array;

    mutable utils::SegmentedArrayVector<Element> m_entries;

public:
    explicit PerIndexArray(const std::vector<Element>& default_array)
      : m_default_array(default_array),
        m_entries(default_array.size()) { }

    ArrayView<Element> operator[](int index) {
        // state_id does not fit anymore so we must resize
        if (static_cast<int>(m_entries.size()) <= index) {
            m_entries.resize(index + 1, m_default_array.data());
        }
        return ArrayView<Element>(m_entries[index], m_default_array.size());
    }
};

}

#endif