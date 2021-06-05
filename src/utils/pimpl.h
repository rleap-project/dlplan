// Code by Herb Sutter, taken from https://herbsutter.com/gotw/_101/
// We follow the rules describes in https://herbsutter.com/gotw/_100/:
//   1. private and nonvirtual functions should be in the pimpl class.
//   2. virtuals that need to be visible to derived classes are part of the public interface.

#ifndef DLP_SRC_UTILS_PIMPL_H_
#define DLP_SRC_UTILS_PIMPL_H_

#include <memory>


namespace dlp {

/**
 * Unique_ptr version without automatically generated copy constructor and copy assignment.
 */
template<typename T>
class pimpl {
private:
    std::unique_ptr<T> m;

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

    // A baseline copy constructor.
    // pimpl(const pimpl<T> &other) : m( new T(*(other.m))) { }

    ~pimpl() { }

    T* operator->() { return m.get(); }

    T& operator*() { return *m.get(); }

    const T* operator->() const { return m.get(); }

    const T& operator*() const { return *m.get(); }
};

/**
 * Shared_ptr version where automatically generated copy constructor and copy assignment copies the shared_ptr.
 */
template<typename T>
class spimpl {
private:
    std::shared_ptr<T> m;

public:
    spimpl() : m{ new T{} } { }

    // Variadic templates
    template<typename Arg1>
    spimpl( Arg1&& arg1 )
        : m( new T( std::forward<Arg1>(arg1) ) ) { }

    template<typename Arg1, typename Arg2>
    spimpl( Arg1&& arg1, Arg2&& arg2 )
        : m( new T( std::forward<Arg1>(arg1), std::forward<Arg2>(arg2) ) ) { }

    template<typename Arg1, typename Arg2, typename Arg3>
    spimpl( Arg1&& arg1, Arg2&& arg2, Arg3&& arg3 )
        : m( new T( std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), std::forward<Arg3>(arg3) ) ) { }

    // A baseline copy constructor.
    // pimpl(const pimpl<T> &other) : m( new T(*(other.m))) { }

    ~spimpl() { }

    T* operator->() { return m.get(); }

    T& operator*() { return *m.get(); }

    const T* operator->() const { return m.get(); }

    const T& operator*() const { return *m.get(); }
};


}

#endif