#ifndef DLPLAN_SRC_CORE_PER_ELEMENT_EVALUATION_CACHES_H_
#define DLPLAN_SRC_CORE_PER_ELEMENT_EVALUATION_CACHES_H_

#include <memory>
#include <unordered_map>

#include "../../include/dlplan/core.h"


namespace dlplan::core {

class PerElementEvaluationCacheImpl {
private:
    std::shared_ptr<const InstanceInfo> m_instance_info;
    int m_num_objects;

    /**
     * An evaluation is cached iff there is an entry
     * in one of the following maps.
     */
    std::unordered_map<int, int> m_concept_index_to_cache_index;
    std::unordered_map<int, int> m_role_index_to_cache_index;

    /**
     * The current strategy is to never deallocate memory when clearing the cache.
     */
    utils::PerIndexBitset m_concept_denotation_cache;
    utils::PerIndexBitset m_role_denotation_cache;

public:
    explicit PerElementEvaluationCacheImpl(std::shared_ptr<const InstanceInfo> instance_info);

    ConceptDenotation retrieve_or_evaluate(PerElementEvaluationCache* parent, const State& state, const element::Concept& concept);

    RoleDenotation retrieve_or_evaluate(PerElementEvaluationCache* parent, const State& state, const element::Role& role);

    void clear();

    std::shared_ptr<const InstanceInfo> get_instance_info() const;
};

}

#endif