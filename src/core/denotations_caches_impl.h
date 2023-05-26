#ifndef DLPLAN_SRC_CORE_DENOTATIONS_CACHES_H_
#define DLPLAN_SRC_CORE_DENOTATIONS_CACHES_H_

#include "../../include/dlplan/core.h"

#include <unordered_map>
#include <unordered_set>
#include <memory>


namespace dlplan::core {

class DenotationsCachesImpl {
private:
    /// @brief Compares two std::unique_ptr<T> by comparing objects T.
    /// @tparam T the nested type
    template<typename T>
    struct DerefEqual {
        bool operator()(const std::unique_ptr<T>& left, const std::unique_ptr<T>& right) const {
            return *left == *right;
        }
    };

    // Cache for single denotations.
    std::unordered_set<std::unique_ptr<ConceptDenotation>, std::hash<std::unique_ptr<ConceptDenotation>>, DerefEqual<ConceptDenotation>> m_c_denot_cache;
    std::unordered_set<std::unique_ptr<RoleDenotation>, std::hash<std::unique_ptr<RoleDenotation>>, DerefEqual<RoleDenotation>> m_r_denot_cache;
    // Cache for collections of denotations.
    std::unordered_set<std::unique_ptr<BooleanDenotations>, std::hash<std::unique_ptr<BooleanDenotations>>, DerefEqual<BooleanDenotations>> m_b_denots_cache;
    std::unordered_set<std::unique_ptr<NumericalDenotations>, std::hash<std::unique_ptr<NumericalDenotations>>, DerefEqual<NumericalDenotations>> m_n_denots_cache;
    std::unordered_set<std::unique_ptr<ConceptDenotations>, std::hash<std::unique_ptr<ConceptDenotations>>, DerefEqual<ConceptDenotations>> m_c_denots_cache;
    std::unordered_set<std::unique_ptr<RoleDenotations>, std::hash<std::unique_ptr<RoleDenotations>>, DerefEqual<RoleDenotations>> m_r_denots_cache;
    // Mapping from element index to denotations.
    std::unordered_map<int, BooleanDenotations*> m_b_denots_mapping;
    std::unordered_map<int, NumericalDenotations*> m_n_denots_mapping;
    std::unordered_map<int, ConceptDenotations*> m_c_denots_mapping;
    std::unordered_map<int, RoleDenotations*> m_r_denots_mapping;
    // Mapping from instance, state, element index to denotations
    std::unordered_map<std::array<int, 3>, int> m_n_denots_mapping_per_state;
    std::unordered_map<std::array<int, 3>, bool> m_b_denots_mapping_per_state;
    std::unordered_map<std::array<int, 3>, ConceptDenotation*> m_c_denots_mapping_per_state;
    std::unordered_map<std::array<int, 3>, RoleDenotation*> m_r_denots_mapping_per_state;

public:
    DenotationsCachesImpl();
    ~DenotationsCachesImpl();
    DenotationsCachesImpl(DenotationsCachesImpl&& other);
    DenotationsCachesImpl& operator=(DenotationsCachesImpl&& other);

    /// Caches concept and role denotations for ensuring uniqueness.
    template<typename T>
    const T* insert_denotation(T&& denotation) {
        static_assert(std::is_same<T, ConceptDenotation>::value ||
                      std::is_same<T, RoleDenotation>::value ,
                      "Unsupported type for insert_denotation method.");
    }

    /// @brief Caches a collection of denotations per element.
    template<typename T>
    void insert_denotations(int element_index, const T* denotations) {
        static_assert(std::is_same<T, ConceptDenotations>::value  ||
                      std::is_same<T, RoleDenotations>::value  ||
                      std::is_same<T, BooleanDenotations>::value  ||
                      std::is_same<T, NumericalDenotations>::value ,
                      "Unsupported type for insert_denotations method.");
    }

    template<typename T>
    const T* get_denotations(int element_index) const {
        static_assert(std::is_same<T, ConceptDenotations>::value  ||
                      std::is_same<T, RoleDenotations>::value  ||
                      std::is_same<T, BooleanDenotations>::value  ||
                      std::is_same<T, NumericalDenotations>::value ,
                      "Unsupported type for get_denotations method.");
    }

    /// @brief Caches a single denotation per element, instance, and state.
    template<typename T>
    void insert_denotation(int element_index, int instance_index, int state_index, const T* denotation) {
        static_assert(std::is_same<T, ConceptDenotation>::value  ||
                      std::is_same<T, RoleDenotation>::value  ||
                      std::is_same<T, bool>::value  ||
                      std::is_same<T, int>::value ,
                      "Unsupported type for insert_denotations method.");
    }
    template<typename T>
    const T* get_denotation(int element_index, int instance_index, int state_index) const {
        static_assert(std::is_same<T, ConceptDenotation>::value  ||
                      std::is_same<T, RoleDenotation>::value  ||
                      std::is_same<T, bool>::value  ||
                      std::is_same<T, int>::value ,
                      "Unsupported type for get_denotation method.");
    }
};

}

#endif
