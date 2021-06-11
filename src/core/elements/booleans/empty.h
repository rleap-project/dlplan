#ifndef DLP_SRC_CORE_ELEMENTS_BOOLEAN_EMPTY_H_
#define DLP_SRC_CORE_ELEMENTS_BOOLEAN_EMPTY_H_

#include "../boolean.h"


namespace dlp::core::element {

template<typename T>
class EmptyBoolean : public Boolean {
protected:
    const T m_element;

protected:
    const bool& evaluate_impl(const State& state) override {
        m_result = m_element->evaluate(state).empty();
        return m_result;
    }

public:
    EmptyBoolean(const VocabularyInfo& vocabulary, T element) : Boolean(vocabulary, "b_empty"), m_element(element) { }

    int compute_complexity() const override {
        return m_element->compute_complexity() + 1;
    }

    std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_element->compute_repr() << ")";
        return ss.str();
    }
};

}

#endif