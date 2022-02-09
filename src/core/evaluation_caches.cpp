#include "evaluation_caches.h"

#include "elements/concept.h"
#include "elements/role.h"


namespace dlplan::core {

EvaluationCachesImpl::EvaluationCachesImpl(int num_objects)
   : m_num_objects(num_objects),
     m_concept_denotation_cache(std::vector<bool>(num_objects, false)),
     m_role_denotation_cache(std::vector<bool>(num_objects * num_objects, false)) {
}

ConceptDenotation EvaluationCachesImpl::get_concept_denotation(const element::Concept& concept) {
    return ConceptDenotation(m_num_objects, m_concept_denotation_cache[concept.get_index()]);
}

RoleDenotation EvaluationCachesImpl::get_role_denotation(const element::Role& role) {
    return RoleDenotation(m_num_objects, m_role_denotation_cache[role.get_index()]);
}

}