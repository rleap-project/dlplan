#include "../../include/dlplan/core.h"

#include "elements/concept.h"
#include "elements/role.h"


namespace dlplan::core {

PerElementEvaluationCache::PerElementEvaluationCache(int num_objects)
    : m_num_objects(num_objects),
      m_concept_denot_cache(std::vector<bool>(num_objects, false)),
      m_role_denot_cache(std::vector<bool>(num_objects * num_objects, false)) { }

PerElementEvaluationCache::~PerElementEvaluationCache() = default;

ConceptDenotation PerElementEvaluationCache::retrieve_or_evaluate(
    const element::Concept& concept, PerElementEvaluationContext& context) {
    auto insert_result = m_concept_index_to_cache_index.emplace(concept.get_index(), m_concept_index_to_cache_index.size());
    int cache_index = insert_result.first->second;
    int cache_miss = insert_result.second;
    ConceptDenotation result(m_num_objects, m_concept_denot_cache[cache_index]);
    if (cache_miss) {
        concept.evaluate(context, result);
    }
    return result;
}

RoleDenotation PerElementEvaluationCache::retrieve_or_evaluate(
    const element::Role& role, PerElementEvaluationContext& context) {
    auto insert_result = m_role_index_to_cache_index.emplace(role.get_index(), m_role_index_to_cache_index.size());
    int cache_index = insert_result.first->second;
    int cache_miss = insert_result.second;
    RoleDenotation result(m_num_objects, m_role_denot_cache[cache_index]);
    if (cache_miss) {
        role.evaluate(context, result);
    }
    return result;
}

void PerElementEvaluationCache::clear_if_state_changed(std::shared_ptr<State> state) {
    if (state != m_cached_state) {
        m_concept_index_to_cache_index.clear();
        m_role_index_to_cache_index.clear();
        m_cached_state = state;
    }
}

}