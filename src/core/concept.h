#ifndef DLP_SRC_CORE_CONCEPT_H_
#define DLP_SRC_CORE_CONCEPT_H_

#include <algorithm>

#include "element.h"


namespace dlp::core {

class ConceptImpl : public ElementImpl<ConceptDenotation> {
public:
    ConceptImpl(const VocabularyInfo& vocabulary_info, std::shared_ptr<element::Element<ConceptDenotation>>&& element)
    : ElementImpl<ConceptDenotation>(vocabulary_info, std::move(element)) {
        if (!m_element) {
            throw std::runtime_error("ConceptImpl::ConceptImpl - tried to construct Concept from nullptr");
        }
    }
    ~ConceptImpl() override = default;

    const ConceptDenotation& evaluate(const State& state) override {
        if (state.get_instance_info()->get_vocabulary_info().get() != m_vocabulary_info) {
            throw std::runtime_error("ConceptImpl::evaluate - mismatched vocabularies of Concept and State.");
        }
        m_result = m_element->evaluate(state);
        std::sort(m_result.begin(), m_result.end());
        return m_result;
    }
};

}

#endif
