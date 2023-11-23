#ifndef DLPLAN_INCLUDE_DLPLAN_COMMON_BASE_HPP_
#define DLPLAN_INCLUDE_DLPLAN_COMMON_BASE_HPP_

#include <sstream>


namespace dlplan {
/// @brief Represents the abstract base class of an object
///        with functionality for computing string representations.
template<typename Derived>
class Base {
protected:
    int m_index;

    Base(int index) : m_index(index) { }

public:
    ~Base() { }

    /// @brief Equality comparison of two objects based on their derived type.
    bool operator==(const Base& other) const {
        return static_cast<const Derived*>(this)->are_equal_impl(static_cast<const Derived&>(other));
    }

    bool operator!=(const Base& other) const {
        return !(*this == other);
    }

    /// @brief Less comparison of two objects based on their index.
    ///        Since objects of the same type are unique and indices
    ///        are assigned depending on order of creation,
    ///        we obtain deterministic sorting very cheaply.
    bool operator<(const Base& other) const {
        return m_index < other.m_index;
    }

    /// @brief Computes a hash value for this object.
    size_t hash() const {
        return static_cast<const Derived*>(this)->hash_impl();
    }

    /// @brief Overload of the output stream insertion operator (operator<<).
    friend std::ostream& operator<<(std::ostream& os, const Base& element) {
        os << element.str();
        return os;
    }

    /// @brief Compute a string representation of this object.
    void str(std::stringstream& out) const {
        static_cast<const Derived*>(this)->str_impl(out);
    }

    /// @brief Compute a string representation of this object.
    std::string str() const {
        std::stringstream out;
        str(out);
        return out.str();
    }

    /* Getters. */
    int get_index() const { return m_index; }
};

}

#endif
