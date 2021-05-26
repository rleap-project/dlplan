// Code by Herb Sutter, taken from https://herbsutter.com/gotw/_101/

#ifndef DLP_SRC_UTILS_PIMPL_H_
#define DLP_SRC_UTILS_PIMPL_H_

#include <memory>


namespace dlp {

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

    // call the copy constructor of the concrete implementation
    // We need to explicitly declare it for every T that we use
    // to (1) restrict copying and (2) forward declare in header
    // pimpl(const pimpl<T> &other) : m( new T(*(other.m))) { }

    ~pimpl() { }

    T* operator->() { return m.get(); }

    T& operator*() { return *m.get(); }

    const T* operator->() const { return m.get(); }

    const T& operator*() const { return *m.get(); }
};

}

#endif