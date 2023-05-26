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
    // Mapping from instance, element index to denotations
    std::unordered_map<std::array<int, 2>, int> m_n_denots_mapping_per_instance;
    std::unordered_map<std::array<int, 2>, bool> m_b_denots_mapping_per_instance;
    std::unordered_map<std::array<int, 2>, ConceptDenotation*> m_c_denots_mapping_per_instance;
    std::unordered_map<std::array<int, 2>, RoleDenotation*> m_r_denots_mapping_per_instance;

public:
    DenotationsCachesImpl();
    ~DenotationsCachesImpl();
    DenotationsCachesImpl(DenotationsCachesImpl&& other);
    DenotationsCachesImpl& operator=(DenotationsCachesImpl&& other);

    const ConceptDenotation* insert(ConceptDenotation&& denotation);
    const RoleDenotation* insert(RoleDenotation&& denotation);
    const ConceptDenotations* insert(ConceptDenotations&& denotations);
    const RoleDenotations* insert(RoleDenotations&& denotations);
    const BooleanDenotations* insert(BooleanDenotations&& denotations);
    const NumericalDenotations* insert(NumericalDenotations&& denotations);

    const ConceptDenotations* get_concept_denotations(int element_index);
    void insert(int element_index, const ConceptDenotations* denotation);
    const RoleDenotations* get_role_denotations(int element_index);
    void insert(int element_index, const RoleDenotations* denotation);
    const BooleanDenotations* get_boolean_denotations(int element_index);
    void insert(int element_index, const BooleanDenotations* denotation);
    const NumericalDenotations* get_numerical_denotations(int element_index);
    void insert(int element_index, const NumericalDenotations* denotation);

    const ConceptDenotation* get_concept_denotation(int element_index, int instance_index, int state_index);
    void insert(int element_index, int instance_index, int state_index, const ConceptDenotation* denotation);
    const RoleDenotation* get_role_denotation(int element_index, int instance_index, int state_index);
    void insert(int element_index, int instance_index, int state_index, const RoleDenotation* denotation);
    bool get_boolean_denotation(int element_index, int instance_index, int state_index);
    void insert(int element_index, int instance_index, int state_index, bool denotation);
    int get_numerical_denotation(int element_index, int instance_index, int state_index);
    void insert(int element_index, int instance_index, int state_index, int denotation);
};

}

#endif
