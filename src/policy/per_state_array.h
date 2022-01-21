
#include <vector>

#include "../utils/segmented_array_vector.h"


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

    std::vector<utils::SegmentedArrayVector<Element>> m_data;

public:
    PerStateArray(const std::vector<Element>& default_array)
      : m_default_array(default_array) { }
};

}