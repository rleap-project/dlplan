/*
 * Copyright (C) 2023 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef DLPLAN_INCLUDE_DLPLAN_UTILS_UNIQUE_FACTORY_HPP_
#define DLPLAN_INCLUDE_DLPLAN_UTILS_UNIQUE_FACTORY_HPP_

#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <mutex>
#include <iostream>
#include <tuple>
#include <vector>
#include <cassert>


namespace dlplan {
template<typename Key, typename... Ts>
class SharedObjectCache {
private:
    // @brief Hashing of the underlying object.
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
        std::unordered_set<std::shared_ptr<const T>, ValueHash<T>, ValueEqual<T>> unique;
        std::unordered_map<Key, std::shared_ptr<const T>> mapping;
    };

    std::tuple<PerTypeCache<Ts>...> m_cache;

public:
    SharedObjectCache()  { }

    template<typename T>
    std::shared_ptr<const T> get(const Key& key) const {
        auto& t_mapping = std::get<PerTypeCache<T>>(m_cache).mapping;
        auto it = t_mapping.find(key);
        if (it == t_mapping.end()) {
            return nullptr;
        }
        return it->second;
    }

    template<typename T>
    void insert_mapping(const Key& key, std::shared_ptr<const T>& element) {
        auto& t_cache = std::get<PerTypeCache<T>>(m_cache);
        auto& t_mapping = t_cache.mapping;
        if (get<T>(key)) {
            throw std::runtime_error("Must call get first before insertion.");
        }
        t_mapping.emplace(key, element);
    }

    template<typename T>
    std::shared_ptr<const T> insert_unique(T&& object) {
        auto& t_cache = std::get<PerTypeCache<T>>(m_cache);
        auto& t_unique = t_cache.unique;
        auto result = t_unique.insert(std::make_shared<T>(std::move(object)));
        return *result.first;
    }
};

}


#endif