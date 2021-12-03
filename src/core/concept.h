#ifndef DLPLAN_SRC_CORE_CONCEPT_H_
#define DLPLAN_SRC_CORE_CONCEPT_H_

#include <algorithm>

#include "element.h"


namespace dlplan::core {

class ConceptImpl : public ElementImpl<ConceptDenotation> {
public:
    ConceptImpl(const std::shared_ptr<element::Element<ConceptDenotation>>& element) {
        if (!element) {
            throw std::runtime_error("ConceptImpl::ConceptImpl - tried to construct Concept from nullptr");
        }
    }
    ~ConceptImpl() override = default;

    ConceptDenotation evaluate(const Element<ConceptDenotation>* parent, const State& state) const override {
        if (state.get_instance_info()->get_vocabulary_info() != parent->get_vocabulary_info()) {
            throw std::runtime_error("ConceptImpl::evaluate - mismatched vocabularies of Concept and State.");
        }
        ConceptDenotation result = parent->get_element()->evaluate(state);
        std::sort(result.begin(), result.end());
        return result;
    }
};

}

#endif
