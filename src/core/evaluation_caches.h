#ifndef DLPLAN_SRC_CORE_EVALUATION_CACHES_H_
#define DLPLAN_SRC_CORE_EVALUATION_CACHES_H_


#include "../../include/dlplan/core.h"


namespace dlplan::core {

class EvaluationCachesImpl {
private:
    int m_num_objects;

    PerStateBitset m_concept_denotation_cache;
    PerStateBitset m_role_denotation_cache;

public:
    explicit EvaluationCachesImpl(int num_objects);

    ConceptDenotation get_concept_denotation(const element::Concept& concept);

    RoleDenotation get_role_denotation(const element::Role& role);
};

}

#endif
