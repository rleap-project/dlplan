#ifndef DLPLAN_SRC_UTILS_SET_OPERATORS_H_
#define DLPLAN_SRC_UTILS_SET_OPERATORS_H_


#include <memory>
#include <unordered_set>


namespace dlplan::utils {

template<typename T>
bool is_subset_eq(const std::unordered_set<T> &left, const std::unordered_set<T>& right)
{
    for (const auto& element : left) {
        if (!right.count(element)) {
            return false;
        }
    }
    return true;
}


template<typename T>
bool is_subset_eq(const std::vector<T> &left, const std::vector<T>& right)
{
    std::unordered_set<T> right_set(right.begin(), right.end());
    for (const auto& element : left) {
        if (!right_set.count(element)) {
            return false;
        }
    }
    return true;
}

template<typename T>
std::vector<T> set_difference(const std::vector<T>& left, const std::vector<T>& right) {
    std::unordered_set<T> right_set(right.begin(), right.end());
    std::vector<T> result;
    for (const auto& element : left) {
        if (!right_set.count(element)) {
            result.push_back(element);
        }
    }
    result.shrink_to_fit();
    return result;
}

template<typename T>
std::unordered_set<T> set_difference(const std::unordered_set<T>& left, const std::unordered_set<T>& right) {
    std::unordered_set<T> result;
    for (const auto& element : left) {
        if (!right.count(element)) {
            result.insert(element);
        }
    }
    return result;
}


template<typename T>
static std::vector<T> set_symmetric_difference(
    const std::vector<std::vector<T>>& sets) {
    std::unordered_map<T, int> frequencies;
    for (const auto& set : sets) {
        for (const auto& element : set) {
            if (frequencies.count(element)) {
                ++frequencies[element];
            } else {
                frequencies[element] = 1;
            }
        }
    }
    std::vector<T> symmetric_diff;
    symmetric_diff.reserve(frequencies.size());
    for (const auto& p : frequencies) {
        if (frequencies[p.first] == 1) {
            symmetric_diff.push_back(p.first);
        }
    }
    symmetric_diff.shrink_to_fit();
    return symmetric_diff;
}

}

#endif
