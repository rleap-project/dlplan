#ifndef DLPLAN_SRC_CORE_EVALUATION_CACHES_H_
#define DLPLAN_SRC_CORE_EVALUATION_CACHES_H_


#include "../../include/dlplan/core.h"


namespace dlplan::core {

class EvaluationCachesImpl {
private:
    int m_num_objects;

    std::unordered_map<int, int> m_concept_index_to_cache_index;
    std::unordered_map<int, int> m_role_index_to_cache_index;

    PerStateBitset m_concept_denotation_cache;
    PerStateBitset m_role_denotation_cache;

public:
    explicit EvaluationCachesImpl(int num_objects);

    std::pair<ConceptDenotation, bool> get_concept_denotation(const element::Concept& concept);

    std::pair<RoleDenotation, bool> get_role_denotation(const element::Role& role);

    void clear();
};

}

#endif
