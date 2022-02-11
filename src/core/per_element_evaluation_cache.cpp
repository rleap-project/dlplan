#include "per_element_evaluation_cache.h"

#include "elements/concept.h"
#include "elements/role.h"


namespace dlplan::core {

PerElementEvaluationCacheImpl::PerElementEvaluationCacheImpl(std::shared_ptr<const InstanceInfo> instance_info)
   : m_instance_info(instance_info),
     m_num_objects(std::max(1, instance_info->get_num_objects())),  // HACK: Using Bitset of size 0 causes Floating Point Exception probably due to division by 0.
     m_concept_denotation_cache(std::vector<bool>(m_num_objects, false)),
     m_role_denotation_cache(std::vector<bool>(m_num_objects * m_num_objects, false)) {
}

ConceptDenotation PerElementEvaluationCacheImpl::retrieve_or_evaluate(PerElementEvaluationCache* parent, const State& state, const element::Concept& concept) {
    int concept_index = concept.get_index();
    auto insert_result = m_concept_index_to_cache_index.emplace(concept_index, m_concept_index_to_cache_index.size());
    int cache_index = insert_result.first->second;
    bool cache_miss = insert_result.second;
    //std::cout << "EvaluationCache: " << concept_index << " " << cache_miss << std::endl;
    ConceptDenotation result(m_num_objects, m_concept_denotation_cache[cache_index]);
    cache_miss = true;
    if (cache_miss) {
        concept.evaluate(state, *parent, result);
    }
    return result;
}

RoleDenotation PerElementEvaluationCacheImpl::retrieve_or_evaluate(PerElementEvaluationCache* parent, const State& state, const element::Role& role) {
    int role_index = role.get_index();
    auto insert_result = m_role_index_to_cache_index.emplace(role_index, m_role_index_to_cache_index.size());
    int cache_index = insert_result.first->second;
    bool cache_miss = insert_result.second;
    //std::cout << "EvaluationCache: " << role_index << " " << cache_miss << std::endl;
    RoleDenotation result(m_num_objects, m_role_denotation_cache[cache_index]);
    cache_miss = true;
    if (cache_miss) {
        role.evaluate(state, *parent, result);
    }
    return result;
}

void PerElementEvaluationCacheImpl::clear() {
    m_concept_index_to_cache_index.clear();
    m_role_index_to_cache_index.clear();
}

std::shared_ptr<const InstanceInfo> PerElementEvaluationCacheImpl::get_instance_info() const {
    return m_instance_info;
}

}