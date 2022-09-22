#ifndef DLPLAN_INCLUDE_DLPLAN_UTILS_POINTER_CACHE_H_
#define DLPLAN_INCLUDE_DLPLAN_UTILS_POINTER_CACHE_H_

#include <unordered_set>
#include <memory>

namespace dlplan::utils {

/**
 * PointerCache stores entry through pointers
 * which makes copying cheap and ptrs remain valid
 * as long as the cache is not destructed.
 */
template<typename T>
class PointerCache {
public:
    template<typename... Args>
    std::unique_ptr<T> get_new_entry(Args... args) const {
        return std::make_unique<T>(T(args...));
    }

    auto insert(std::unique_ptr<T>&& entry) {
        return m_storage.insert(std::move(entry));
    }
private:
    struct PointerCacheHash {
        std::size_t operator()(const std::unique_ptr<T>& entry) const {
            return std::hash<std::unique_ptr<T>>()(entry);
        }
    };

    struct PointerCacheEqual {
        bool operator()(const std::unique_ptr<T>& left, const std::unique_ptr<T>& right) const {
            return *left == *right;
        }
    };
private:
    std::unordered_set<std::unique_ptr<T>, PointerCacheHash, PointerCacheEqual> m_storage;
};

}


#endif
