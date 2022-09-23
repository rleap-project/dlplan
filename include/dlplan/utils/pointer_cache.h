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
template<typename T, typename Hash, typename Equal>
class PointerCache {
public:
    template<typename... Args>
    std::unique_ptr<T> get_new_entry(Args&& ...args) const {
        return std::make_unique<T>(T(std::forward<Args>(args)...));
    }

    auto insert(std::unique_ptr<T>&& entry) {
        return m_storage.insert(std::move(entry));
    }
private:
    std::unordered_set<std::unique_ptr<T>, Hash, Equal> m_storage;
};

}


#endif
