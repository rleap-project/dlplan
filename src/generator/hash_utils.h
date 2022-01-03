#ifndef DLPLAN_SRC_GENERATOR_HASH_UTILS_H_
#define DLPLAN_SRC_GENERATOR_HASH_UTILS_H_

#include <vector>
#include <array>

#include "types.h"

#include "../utils/MurmurHash3.h"


namespace dlplan {
namespace core {
template<typename T>
class Element;
}
namespace generator {

/**
 * For combining hash value we use the boost::hash_combine one-liner.
 * https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector
 */
template <class T>
void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

std::array<uint32_t, 4> compute_hash(const std::vector<int>& denotation) {
    std::array<uint32_t, 4> a;
    MurmurHash3_x86_128(denotation.begin().base(), sizeof(int) * denotation.size(), denotation.size(), a.begin());
    return a;
}

template<typename T>
static void print_elements(const std::vector<std::vector<T>>& elements_by_complexity) {
    for (const auto& elements : elements_by_complexity) {
        for (const auto& element : elements) {
            std::cout << element.compute_complexity() << " " << element.compute_repr() << std::endl;
        }
    }
}

/**
 * Evaluates an element on a collection of states.
 */
template<typename D>
std::vector<D> evaluate(core::Element<D>& element, const States& states) {
    std::vector<D> result;
    result.reserve(states.size());
    for (const auto& state : states) {
        result.push_back(element.evaluate(state));
    }
    result.shrink_to_fit();
    return result;
}

std::vector<int> bool_vec_to_num_vec(const std::vector<bool>& bool_vec) {
    std::vector<int> num_vec;
    num_vec.reserve(bool_vec.size());
    for (size_t i = 0; i < bool_vec.size(); ++i) {
        num_vec.push_back(bool_vec[i]);
    }
    return num_vec;
}

template<typename T>
std::vector<int> bitset_to_num_vec(const std::vector<T>& denotation) {
    static_assert(sizeof(int) == sizeof(unsigned));
    size_t size = 0;
    for (const auto& b : denotation) {
        size += b.get_const_data().get_blocks().size();
    }
    std::vector<int> result;
    result.reserve(size);
    for (const auto& b : denotation) {
        result.insert(result.end(), b.get_const_data().get_blocks().begin(), b.get_const_data().get_blocks().end());
    }
    return result;
}

}
}


namespace std {
    /**
     * We provide custom specialization of std::hash that are injected in the namespace std.
     * https://en.cppreference.com/w/cpp/utility/hash
     */
    template<> struct hash<std::vector<int>> {
        std::size_t operator()(const std::vector<int>& denotation) const noexcept {
            std::size_t seed = denotation.size();
            for (const auto& i : denotation) {
                dlplan::generator::hash_combine(seed, i);
            }
            return seed;
        }
    };
    template<> struct hash<std::array<uint32_t, 4>> {
        std::size_t operator()(const std::array<uint32_t, 4>& h) const noexcept {
            std::size_t seed = 4;
            for (int i = 0; i < 4; ++i) {
                dlplan::generator::hash_combine(seed, h[i]);
            }
            return seed;
        }
    };
}

#endif
