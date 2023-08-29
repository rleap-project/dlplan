// Code by Herb Sutter, taken from https://herbsutter.com/gotw/_101/
// We follow option 4 (10/10) described in https://herbsutter.com/gotw/_100/:
//   * Put all private nonvirtual members into impl;

#ifndef DLPLAN_INCLUDE_DLPLAN_UTILS_PIMPL_H_
#define DLPLAN_INCLUDE_DLPLAN_UTILS_PIMPL_H_

#include <memory>

#include <boost/serialization/unique_ptr.hpp>


// Forward declarations of this header.
namespace dlplan::utils {
template<typename T>
class pimpl;
}

// Forward declare the serialize function template in boost::serialization namespace
namespace boost::serialization {
    template <typename Archive, typename T>
    void serialize(Archive& ar, dlplan::utils::pimpl<T>& factory, const unsigned int version);
}


namespace dlplan::utils {
template<typename T>
class pimpl {
private:
    std::unique_ptr<T> m;

    template<typename Archive, typename T_>
    friend void boost::serialization::serialize(Archive& ar, pimpl<T_>& pimpl, const unsigned int version);

public:
    pimpl() : m{ new T{} } { }

    // Variadic templates
    template<typename Arg1>
    pimpl( Arg1&& arg1 )
        : m( new T( std::forward<Arg1>(arg1) ) ) { }

    template<typename Arg1, typename Arg2>
    pimpl( Arg1&& arg1, Arg2&& arg2 )
        : m( new T( std::forward<Arg1>(arg1), std::forward<Arg2>(arg2) ) ) { }

    template<typename Arg1, typename Arg2, typename Arg3>
    pimpl( Arg1&& arg1, Arg2&& arg2, Arg3&& arg3 )
        : m( new T( std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), std::forward<Arg3>(arg3) ) ) { }

    ~pimpl() = default;

    T* operator->() { return m.get(); }

    T& operator*() { return *m.get(); }

    const T* operator->() const { return m.get(); }

    const T& operator*() const { return *m.get(); }
};

}


namespace boost::serialization {
template<typename Archive, typename T>
void serialize(Archive& ar, dlplan::utils::pimpl<T>& t, const unsigned int /* version */ )
{
    ar & t.m;
}

}

#endif