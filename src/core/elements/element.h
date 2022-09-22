#ifndef DLPLAN_SRC_CORE_ELEMENTS_ELEMENT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ELEMENT_H_

#include <iostream>
#include <sstream>
#include <mutex>
#include <cassert>
#include <memory>

#include "types.h"

#include "../../../include/dlplan/utils/cache.h"
#include "../../../include/dlplan/utils/pointer_cache.h"
#include "../../../include/dlplan/core.h"


using namespace std::string_literals;

namespace dlplan::core::element {
class DenotationsCaches;

template<typename T>
using DENOTS = std::vector<T>;

template<typename T>
class Element : public utils::Cachable {
protected:
    /**
     * Index can be used for internal caching.
     */
    int m_index;

public:
    // Elements are not copieable because they must live in the cache.
    explicit Element(const VocabularyInfo&) : m_index(-1) { }
    Element(const Element& other) = delete;
    Element& operator=(const Element& other) = delete;
    Element(Element&& other) = delete;
    Element& operator=(Element&& other) = delete;
    virtual ~Element() = default;

    virtual int compute_complexity() const = 0;

    virtual void compute_repr(std::stringstream& out) const = 0;

    void set_index(int index) {
        m_index = index;
    }

    virtual std::string compute_repr() const {
        std::stringstream ss;
        compute_repr(ss);
        return ss.str();
    }

    int get_index() const {
        return m_index;
    }
};


template<typename T>
class DenotationsMapping {
private:
    std::unordered_map<int, T*> m_mapping;

public:
    T* insert(T* denotations, int element_index) {
        return m_mapping.insert(std::make_pair(element_index, denotations)).first->second;
    }

    T* find(int element_index) const {
        auto result = m_mapping.find(element_index);
        if (result == m_mapping.end())
            return nullptr;
        return result->second;
    }
};


struct DenotationsCaches {
    // Cache for single denotations.
    dlplan::utils::PointerCache<ConceptDenotation> m_c_denot_cache;
    dlplan::utils::PointerCache<RoleDenotation> m_r_denot_cache;
    // Cache for collections of denotations.
    dlplan::utils::PointerCache<std::vector<bool>> m_b_denots_cache;
    dlplan::utils::PointerCache<std::vector<int>> m_n_denots_cache;
    dlplan::utils::PointerCache<std::vector<ConceptDenotation*>> m_c_denots_cache;
    dlplan::utils::PointerCache<std::vector<RoleDenotation*>> m_r_denots_cache;
    // Mapping from element index to denotations.
    DenotationsMapping<std::vector<bool>> m_b_denots_mapping;
    DenotationsMapping<std::vector<int>> m_n_denots_mapping;
    DenotationsMapping<std::vector<ConceptDenotation*>> m_c_denots_mapping;
    DenotationsMapping<std::vector<RoleDenotation*>> m_r_denots_mapping;
};

}


#endif
