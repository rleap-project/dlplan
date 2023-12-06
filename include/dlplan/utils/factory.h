#ifndef DLPLAN_SRC_UTILS_FACTORY_H_
#define DLPLAN_SRC_UTILS_FACTORY_H_


#include <unordered_map>
#include <memory>
#include <mutex>
#include <iostream>
#include <utility>


namespace dlplan {
template<typename T>
struct ValueHash {
    std::size_t operator()(const std::shared_ptr<const T>& ptr) const {
        return std::hash<T>()(*ptr);
    }
};

/// @brief Equality comparison of the objects underlying the pointers.
template<typename T>
struct ValueEqual {
    bool operator()(const std::shared_ptr<const T>& left, const std::shared_ptr<const T>& right) const {
        return *left == *right;
    }
};

template<typename T>
struct PerTypeCache {
    std::unordered_map<std::shared_ptr<T>, std::weak_ptr<T>, ValueHash<T>, ValueEqual<T>> data;
    std::unordered_map<int, std::shared_ptr<T>> identifier_to_key;
};

template<typename T>
struct GetOrCreateResult {
    std::shared_ptr<const T> object;
    bool created;
};


/// @brief A reference-counted object cache.
/// Original idea by Herb Sutter.
/// Custom deleter idea: https://stackoverflow.com/questions/49782011/herb-sutters-10-liner-with-cleanup
template<typename... Ts>
class ReferenceCountedObjectFactory {
private:
    std::tuple<std::shared_ptr<PerTypeCache<Ts>>...> m_cache;

    // Identifiers are shared since types can be polymorphic
    int m_count = -1;

public:
    ReferenceCountedObjectFactory()
        : m_cache((std::make_shared<PerTypeCache<Ts>>())...) { }

    /// @brief Gets a shared reference to the object of type T with the given arguments.
    ///        If such an object does not exists then it creates one.
    /// @tparam ...Args The arguments that are passed to the constructor of T.
    /// @param ...args
    /// @return
    template<typename T, typename... Args>
    GetOrCreateResult<T> get_or_create(Args&&... args) {
        auto& t_cache = std::get<std::shared_ptr<PerTypeCache<T>>>(m_cache);
        int identifier = ++m_count;
        /* Must explicitly call the constructor of T to give exclusive access to the factory. */
        auto key = std::shared_ptr<T>(new T(identifier, std::forward<Args>(args)...));
        /* we must declare sp before locking the mutex
           s.t. the deleter is called after the mutex was released in case of stack unwinding. */
        std::shared_ptr<T> sp;
        auto& cached = t_cache->data[key];
        sp = cached.lock();
        // std::lock_guard<std::mutex> hold(t_cache->mutex);
        bool new_insertion = false;

        if (!sp) {
            new_insertion = true;
            t_cache->identifier_to_key.emplace(identifier, key);
            cached = sp = std::shared_ptr<T>(
                new T(identifier, std::forward<Args>(args)...),
                [parent=t_cache, identifier](T* x)
                {
                    {
                        // std::lock_guard<std::mutex> hold(parent->mutex);
                        const auto& key = parent->identifier_to_key.at(identifier);
                        parent->data.erase(key);
                        parent->identifier_to_key.erase(identifier);
                    }
                    delete x;
                }
            );
        }
        return GetOrCreateResult<T>{sp, new_insertion};
    }
};

}

#endif