#ifndef DLPLAN_SRC_CORE_NUMERICAL_H_
#define DLPLAN_SRC_CORE_NUMERICAL_H_

#include "element.h"


namespace dlplan::core {

class NumericalImpl : public ElementImpl<int> {
public:
    NumericalImpl(const VocabularyInfo& vocabulary_info, std::shared_ptr<element::Element<int>>&& element)
    : ElementImpl<int>(vocabulary_info, std::move(element)) {
        if (!m_element) {
            throw std::runtime_error("NumericalImpl::NumericalImpl - tried to construct Numerical from nullptr");
        }
    }
    ~NumericalImpl() override = default;

    const int& evaluate(const State& state) override {
        if (state.get_instance_info()->get_vocabulary_info().get() != m_vocabulary_info) {
            throw std::runtime_error("NumericalImpl::evaluate - mismatched vocabularies of Numerical and State.");
        }
        return m_element->evaluate(state);
    }
};

}

#endif
