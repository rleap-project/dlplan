#include "evaluation_caches.h"

#include "elements/concept.h"
#include "elements/role.h"


namespace dlplan::core {

EvaluationCachesImpl::EvaluationCachesImpl(int num_objects)
   : m_num_objects(num_objects),
     m_concept_denotation_cache(std::vector<bool>(num_objects, false)),
     m_role_denotation_cache(std::vector<bool>(num_objects * num_objects, false)) {
}

std::pair<ConceptDenotation, bool> EvaluationCachesImpl::get_concept_denotation(const element::Concept& concept) {
    int concept_index = concept.get_index();
    auto insert_result = m_concept_index_to_cache_index.emplace(concept_index, m_concept_index_to_cache_index.size());
    int cache_index = insert_result.first->second;
    bool cache_status = insert_result.second;
    return std::make_pair(ConceptDenotation(m_num_objects, m_concept_denotation_cache[cache_index]), cache_status);
}

std::pair<RoleDenotation, bool> EvaluationCachesImpl::get_role_denotation(const element::Role& role) {
    int role_index = role.get_index();
    auto insert_result = m_role_index_to_cache_index.emplace(role_index, m_role_index_to_cache_index.size());
    int cache_index = insert_result.first->second;
    bool cache_status = insert_result.second;
    return std::make_pair(RoleDenotation(m_num_objects, m_role_denotation_cache[cache_index]), cache_status);
}

void EvaluationCachesImpl::clear() {
    m_concept_index_to_cache_index.clear();
    m_role_index_to_cache_index.clear();
}

}