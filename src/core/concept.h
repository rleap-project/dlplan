#ifndef DLPLAN_SRC_CORE_CONCEPT_H_
#define DLPLAN_SRC_CORE_CONCEPT_H_

#include <memory>

#include "elements/concept.h"

#include "../../include/dlplan/core.h"


namespace dlplan::core {

class ConceptImpl {
public:
    ConceptImpl(std::shared_ptr<const element::Concept>& element) {
        if (!element) {
            throw std::runtime_error("ConceptImpl::ConceptImpl - tried to construct Concept from nullptr");
        }
    }

    ConceptDenotation evaluate(const Concept* parent, const State& state, PerElementEvaluationCache& caches) const {
        if (state.get_instance_info()->get_vocabulary_info() != parent->get_vocabulary_info()) {
            throw std::runtime_error("ConceptImpl::evaluate - mismatched vocabularies of Concept and State.");
        }
        if (state.get_instance_info() != caches.get_instance_info()) {
            throw std::runtime_error("ConceptImpl::evaluate - mismatches instances of State and EvaluationCache.");
        }
        return parent->get_element()->evaluate(state, caches);
    }
};

}

#endif
