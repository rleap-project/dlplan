#ifndef DLPLAN_SRC_UTILS_CACHE_H_
#define DLPLAN_SRC_UTILS_CACHE_H_

#include <unordered_map>
#include <memory>
#include <mutex>
#include <iostream>

#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/weak_ptr.hpp>


namespace dlplan::utils {
template<typename KEY, typename VALUE>
class ReferenceCountedObjectCache;
}

namespace boost::serialization {
    template<typename Archive, typename KEY, typename VALUE>
    void serialize(Archive& ar, dlplan::utils::ReferenceCountedObjectCache<KEY, VALUE>& cache, const unsigned int version);
}


namespace dlplan::utils {

/**
 * A thread-safe reference-counted object cache.
 * Idea taken from Herb Sutter: https://channel9.msdn.com/Events/GoingNative/2013/My-Favorite-Cpp-10-Liner
 * Other sources: (1) https://stackoverflow.com/questions/49782011/herb-sutters-10-liner-with-cleanup
 */
template<typename KEY, typename VALUE>
class ReferenceCountedObjectCache : public std::enable_shared_from_this<ReferenceCountedObjectCache<KEY, VALUE>> {
private:
    std::unordered_map<KEY, std::weak_ptr<VALUE>> m_cache;

    /**
     * For multi-threading purposes
     */
    mutable std::mutex m_mutex;

    template<typename Archive, typename KEY_, typename VALUE_>
    friend void boost::serialization::serialize(Archive& ar, ReferenceCountedObjectCache<KEY_, VALUE_>& cache, const unsigned int version);

public:
    ReferenceCountedObjectCache() { }
    /**
     * Retrieves a certain element.
     */
    std::shared_ptr<VALUE> at(const KEY& key) {
        std::lock_guard<std::mutex> hold(m_mutex);
        return m_cache.at(key).lock();
    }

    /**
     * Inserts a new value and derives the key from it.
     */
    std::pair<std::shared_ptr<VALUE>, bool> insert(std::unique_ptr<VALUE>&& element) {
        KEY key = element->compute_repr();
        /* we must declare sp before locking the mutex
           s.t. the deleter is called after the mutex was released in case of stack unwinding. */
        std::shared_ptr<VALUE> sp;
        std::lock_guard<std::mutex> hold(m_mutex);
        auto& cached = m_cache[key];
        sp = cached.lock();
        bool new_insertion = false;
        if (!sp) {
            new_insertion = true;
            cached = sp = std::shared_ptr<VALUE>(
                element.get(),
                [parent=this->shared_from_this(), original_deleter=element.get_deleter()](VALUE* x)
                {
                    {
                        std::lock_guard<std::mutex> hold(parent->m_mutex);
                        parent->m_cache.erase(x->compute_repr());
                    }
                    /* After cache removal, we can call the objects destructor
                       and recursively call the deleter of children if their ref count goes to 0 */
                    original_deleter(x);
                }
            );
            element.release();
        }
        return std::make_pair(sp, new_insertion);
    }

    /**
     * Inserts a new (key, value) pair
     */
    //std::pair<std::shared_ptr<VALUE>, bool> insert(KEY&& key, std::unique_ptr<VALUE>&& element) {
    //    /* we must declare sp before locking the mutex
    //       s.t. the deleter is called after the mutex was released in case of stack unwinding. */
    //    std::shared_ptr<VALUE> sp;
    //    std::lock_guard<std::mutex> hold(m_mutex);
    //    auto& cached = m_cache[key];
    //    sp = cached.lock();
    //    bool new_insertion = false;
    //    if (!sp) {
    //        new_insertion = true;
    //        cached = sp = std::shared_ptr<VALUE>(
    //            element.get(),
    //            // the key is stored in the field adding extra overhead.
    //            [key, parent=this->shared_from_this(), original_deleter=element.get_deleter()](VALUE* x)
    //            {
    //                {
    //                    std::lock_guard<std::mutex> hold(parent->m_mutex);
    //                    parent->m_cache.erase(key);
    //                }
    //                /* After cache removal, we can call the objects destructor
    //                   and recursively call the deleter of children if their ref count goes to 0 */
    //                original_deleter(x);
    //            }
    //        );
    //        element.release();
    //    }
    //    return std::make_pair(sp, new_insertion);
    //}

    size_t size() const {
        std::lock_guard<std::mutex> hold(m_mutex);
        return m_cache.size();
    }
};

}

namespace boost::serialization {
template<typename Archive, typename KEY, typename VALUE>
void serialize(Archive& ar, dlplan::utils::ReferenceCountedObjectCache<KEY, VALUE>& t, const unsigned int /* version */ )
{
    ar & t.m_cache;
}

}

#endif