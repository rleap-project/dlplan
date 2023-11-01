#ifndef DLPLAN_SRC_WEISFEILER_LEHMAN_COLOR_H_
#define DLPLAN_SRC_WEISFEILER_LEHMAN_COLOR_H_

#include <set>

#include "../../include/dlplan/weisfeiler_lehman.h"


namespace dlplan::weisfeiler_lehman {

class Color : public std::multiset<CompressedColor> {
public:
    using std::multiset<int>::multiset;
    /**
     *
     */
    /**
     * During compression we store Color in an unordered_map.
     * Hence, we need to be able to compute a hash value.
     */
    std::size_t hash() const;
};

}

namespace std {
    /**
     * We provide custom specialization of std::hash that are injected in the namespace std
     * to be able to use standard hash containers.
     * https://en.cppreference.com/w/cpp/utility/hash
     */
    template<> struct hash<dlplan::weisfeiler_lehman::Color> {
        std::size_t operator()(const dlplan::weisfeiler_lehman::Color& color) const noexcept {
            return color.hash();
        }
    };
}

#endif