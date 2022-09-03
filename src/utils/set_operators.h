#include <memory>
#include <unordered_set>


namespace dlplan::utils {

template <typename T>
bool is_subset_eq(const std::unordered_set<T> &left, const std::unordered_set<T>& right)
{
    for (const auto& element : left) {
        if (!right.count(element)) {
            return false;
        }
    }
    return true;
}

}